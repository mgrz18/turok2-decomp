#!/usr/bin/env python3
"""Round 16 e3: a navigation mesh from a level's floor regions, A* between two points, a path the
player can walk (docs/port/PLAYTHROUGH.md section 30).

    python3 tools/port/nav.py ROM LEVEL X,Y,Z X,Y,Z [--jumps]   # the waypoints between two points
    python3 tools/port/nav.py ROM LEVEL --check RUNLOG   # every same-level teleport of a run's log

The floor is the level's collision regions (tools/assets/levels.py: triangles, each with its three edge
neighbours). The player crosses an edge the way Sweep_CrossRegionEdge (func_0023B664.c) lets him:
- a border edge (no neighbour) is a wall;
- a steep region (REGION flag 8, or its normal when flag 0x2000 is set: CGameRegion__IsSteep) is a wall
  when its highest corner is above his feet, so steep regions are never nodes here;
- the region-flag 0x80 and region-info 0x400000 / flags52 0x80 boundaries depend on the mover's
  collision flags: kept closed (conservative);
- deep water (region flag 1 without the info's shallow bit 0x20000000) costs WATER_COST times its length;
- a drop of more than DROP_MAX at the shared edge is one-way and costs DROP_COST.
Static objects and doors (actors) are not in the mesh: the closed-loop walker (aimdrive.py's
WalkDrive) detects being stuck and steps round them.

Pure data: the ROM through tools/assets (levels.parse_level); nothing of the game is run."""
import heapq, math, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "assets"))
import levels as lv  # noqa: E402
import romfs  # noqa: E402

RADIUS = 40.0       # the portals shrink by this at a wall corner (the player's collision radius ~ 30)
DROP_MAX = 120.0    # higher drops are one-way
DROP_COST = 400.0
WATER_COST = 4.0
STEEP_TOL = 2.0     # the steep region's top may be this much above the floor at the edge
STEP_UP = 30.0      # a non-steep neighbour at most this much higher at the shared edge
SLIDE_COST = 200.0
BOX_GROW = 36.0
BOX_MAX = 1000.0    # obstacles: no side over BOX_MAX, and thin (one side up to BOX_THIN) or small (both up
BOX_THIN = 160.0    # to BOX_SMALL); the others are hollow or walked on (their mesh collides, not the box)
BOX_SMALL = 250.0
DETOUR_OUT = 12.0   # a detour corner this much outside the grown box     # the player's radius round a static object's box
BODY_LO, BODY_HI = 56.0, 90.0   # a box in the way: over the feet + BODY_LO and under the feet + BODY_HI
CLIMB_COST = 600.0  # a steep region (a slide down) on the way
FLOOR_TOL = 60.0    # locate(): the floor at most this above the point
# round 19 h5: jump edges (load(..., jumps=True)): the player's jump (PlayerJump_Enter: vy 389.12 x the
# jump scale; measured, PLAYTHROUGH.md 39-h5) lifts his feet JUMP_UP over a ledge he walks into, and a
# running jump carries him GAP_MAX over a void between two floors
JUMP_UP = 90.0      # a ledge (a higher floor or a steep face whose top is) at most this over the feet
GAP_MAX = 150.0     # a void between two floors' border edges at most this wide (XZ)
GAP_UP = 40.0       # the far floor at most this higher across a gap
GAP_DROP = 300.0    # and at most this lower
JUMP_COST = 300.0   # a jump on the way costs this (the walker prefers the floor)

_FS = {}
_NAV = {}


def fs_of(rom):
    if rom not in _FS:
        _FS[rom] = romfs.RomFS(rom)
    return _FS[rom]


def load(rom, level, jumps=False):
    """The Nav of a level (cached). jumps (round 19 h5): with the jump edges (ledges and gaps)."""
    key = (rom, level, bool(jumps))
    if key not in _NAV:
        _NAV[key] = Nav(lv.parse_level(fs_of(rom), level, with_sections=True), jumps=jumps)
    return _NAV[key]


def cross2(o, a, b):
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def seg_cross(p, q, a, b):
    """The XZ point where segment p-q crosses segment a-b, or None."""
    d = (q[0] - p[0]) * (b[1] - a[1]) - (q[1] - p[1]) * (b[0] - a[0])
    if abs(d) < 1e-9:
        return None
    t = ((a[0] - p[0]) * (b[1] - a[1]) - (a[1] - p[1]) * (b[0] - a[0])) / d
    u = ((a[0] - p[0]) * (q[1] - p[1]) - (a[1] - p[1]) * (q[0] - p[0])) / d
    if -1e-6 <= t <= 1 + 1e-6 and -1e-6 <= u <= 1 + 1e-6:
        return (p[0] + (q[0] - p[0]) * t, p[1] + (q[1] - p[1]) * t)
    return None


class Nav:
    def __init__(self, level, jumps=False):
        self.level = level["index"]
        self.jumps = jumps
        col = level["collision"]
        self.v = [tuple(p) for p in col["vertices"]]     # x, floor y, z, ceiling y
        self.infos = col["region_infos"]
        self.r = col["regions"]
        n = len(self.r)
        self.walk = [self._walkable(i) for i in range(n)]
        self.cent = [self._centroid(i) for i in range(n)]
        self.water = [self._deep(i) for i in range(n)]
        # the static objects' boxes are opt-in (T2_NAV_BOXES=1): a box is only the broad phase, and going round
        # one made walks fail that went straight through (135, PLAYTHROUGH.md 30); the walker adds the
        # places it got stuck at instead
        self.boxes = self._boxes(level) if os.environ.get("T2_NAV_BOXES") == "1" else []
        # the boxes do not close regions (a box is only a broad phase: a closed region was often open floor);
        # the legs of a path go round them (detour)
        self.blocked = [False] * n
        self.gaps = self._gap_links() if jumps else {}

    # --- static objects: their world boxes (CStaticObject__Construct, func_00214564.c) ---
    def _boxes(self, level):
        """[(xmin, xmax, zmin, zmax, ymin, ymax)] of the static objects, each grown by RADIUS on the floor."""
        def bf(v):   # HalfToFloat: the top half of a float
            return struct.unpack(">f", struct.pack(">I", (v & 0xFFFF) << 16))[0]
        out = []
        for g in level.get("grid_sections", []):
            d = g.get("data")
            if not d:
                continue
            for o in d["static_objects"]:
                p = o["pos"]
                if o["flags"] & 0x40:
                    e = [-102.4] * 3 + [102.4] * 3
                else:
                    e = [bf(v) for v in o["extents_half"]]
                lo = [p[k] + e[k] - 0.5 for k in range(3)]
                hi = [p[k] + e[k + 3] + 0.5 for k in range(3)]
                dx, dz = hi[0] - lo[0], hi[2] - lo[2]
                if max(dx, dz) > BOX_MAX or (min(dx, dz) > BOX_THIN and max(dx, dz) > BOX_SMALL):
                    continue   # a building, a corridor piece, a mound: hollow or walked on (the box is only
                               # the broad phase); kept: walls (thin), graves, posts and pillars (small)
                out.append((lo[0] - BOX_GROW, hi[0] + BOX_GROW, lo[2] - BOX_GROW, hi[2] + BOX_GROW, lo[1], hi[1]))
        return out

    def _in_box(self, x, z, y):
        """The first box over (x, z) that stands in the way of a player whose feet are at y."""
        for k, b in enumerate(self.boxes):
            if b[0] < x < b[1] and b[2] < z < b[3] and b[4] < y + BODY_HI and b[5] > y + BODY_LO:
                return k
        return None

    def seg_box(self, a, b, y):
        """The first box the XZ segment a-b crosses (feet at y), or None."""
        for k, bx in enumerate(self.boxes):
            if not (bx[4] < y + BODY_HI and bx[5] > y + BODY_LO):
                continue
            # slab test
            t0, t1 = 0.0, 1.0
            ok = True
            for (p, d, lo, hi) in ((a[0], b[0] - a[0], bx[0], bx[1]), (a[1], b[1] - a[1], bx[2], bx[3])):
                if abs(d) < 1e-9:
                    if not lo < p < hi:
                        ok = False
                        break
                    continue
                u0, u1 = (lo - p) / d, (hi - p) / d
                if u0 > u1:
                    u0, u1 = u1, u0
                t0, t1 = max(t0, u0), min(t1, u1)
                if t0 >= t1:
                    ok = False
                    break
            if ok:
                return k
        return None

    # --- regions ---
    def corners(self, i):
        return [self.v[k] for k in self.r[i]["v"]]

    def _centroid(self, i):
        c = self.corners(i)
        return (sum(p[0] for p in c) / 3, sum(p[1] for p in c) / 3, sum(p[2] for p in c) / 3)

    def _steep(self, i):
        fl = self.r[i]["flags"]
        if not fl & 0x2000:
            return bool(fl & 8)
        a, b, c = self.corners(i)
        e0 = (b[0] - a[0], b[1] - a[1], b[2] - a[2])
        e1 = (c[0] - b[0], c[1] - b[1], c[2] - b[2])
        nx = e0[1] * e1[2] - e0[2] * e1[1]
        ny = e0[2] * e1[0] - e0[0] * e1[2]
        nz = e0[0] * e1[1] - e0[1] * e1[0]
        ln = math.sqrt(nx * nx + ny * ny + nz * nz) or 1.0
        return abs(ny / ln) < 0.5   # the engine's threshold is in CGameRegion__IsSteep's tail (cos 60)

    def _walkable(self, i):
        return not self._steep(i)

    def _deep(self, i):
        if not self.r[i]["flags"] & 1:
            return False
        return not self.infos[self.r[i]["info"]]["flags"] & 0x20000000

    def height(self, i, x, z):
        """The floor of region i at (x, z) (its plane)."""
        a, b, c = self.corners(i)
        d = (b[2] - c[2]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[2] - c[2])
        if abs(d) < 1e-9:
            return (a[1] + b[1] + c[1]) / 3
        l1 = ((b[2] - c[2]) * (x - c[0]) + (c[0] - b[0]) * (z - c[2])) / d
        l2 = ((c[2] - a[2]) * (x - c[0]) + (a[0] - c[0]) * (z - c[2])) / d
        return l1 * a[1] + l2 * b[1] + (1 - l1 - l2) * c[1]

    def contains(self, i, x, z, eps=0.5):
        a, b, c = self.corners(i)
        p = (x, z)
        d1 = cross2((a[0], a[2]), (b[0], b[2]), p)
        d2 = cross2((b[0], b[2]), (c[0], c[2]), p)
        d3 = cross2((c[0], c[2]), (a[0], a[2]), p)
        neg = d1 < -eps or d2 < -eps or d3 < -eps
        pos = d1 > eps or d2 > eps or d3 > eps
        return not (neg and pos)

    def locate(self, x, y, z):
        """The walkable region under (x, y, z): the highest floor at most FLOOR_TOL above y; else the
        nearest walkable region's centroid (a point just off the mesh)."""
        best, bh = None, None
        for i in range(len(self.r)):
            if not self.walk[i] or not self.contains(i, x, z):
                continue
            h = self.height(i, x, z)
            if h <= y + FLOOR_TOL and (bh is None or h > bh):
                best, bh = i, h
        if best is not None:
            return best
        cand = [i for i in range(len(self.r)) if self.walk[i]]
        return min(cand, key=lambda i: (self.cent[i][0] - x) ** 2 + (self.cent[i][2] - z) ** 2 +
                   4 * (self.cent[i][1] - y) ** 2) if cand else None

    def edge(self, i, k):
        vs = self.r[i]["v"]
        return self.v[vs[k]], self.v[vs[(k + 1) % 3]]

    def links(self, i):
        """(neighbour, (a, b) the edge crossed, extra cost, kind) of the edges the player can cross out of
        region i; kind "walk", "jump" (a ledge) or "gap" (round 19 h5: only with jumps=True).
        Sweep_CrossRegionEdge: into a steep region (a wall or a slide) only when its highest corner is
        below the feet (here: the floor at the shared edge); out of a steep region anywhere (here: not up a
        steep one); a non-steep floor that does not join at the edge (a step up of more than STEP_UP)
        stays closed too (the engine lets it be crossed; the floor follow then fails). With jumps, a
        steep face or a higher floor at most JUMP_UP over the feet is a "jump" edge, and the gaps of
        _gap_links() are "gap" links."""
        out = []
        ri = self.r[i]
        info_i = self.infos[ri["info"]]
        si = not self.walk[i]
        for k, j in enumerate(ri["neighbours"]):
            if j is None or self.blocked[j]:
                continue
            rj = self.r[j]
            info_j = self.infos[rj["info"]]
            if (ri["flags"] ^ rj["flags"]) & 0x80:
                continue
            if (info_i["flags"] ^ info_j["flags"]) & 0x400000:
                continue
            if (info_i["flags52"] ^ info_j["flags52"]) & 0x80:
                continue
            a, b = self.edge(i, k)
            mx, mz = (a[0] + b[0]) / 2, (a[2] + b[2]) / 2
            hi, hj = self.height(i, mx, mz), self.height(j, mx, mz)
            top_j = max(self.v[q][1] for q in rj["v"])
            sj = not self.walk[j]
            climb = sj and rj["flags"] & 0x10    # a climbable wall (PlayerClimb_Enter, facing it)
            kind = "walk"
            if sj and not si and not climb and top_j >= hi - 0.1 + STEEP_TOL:
                if not (self.jumps and top_j <= hi + JUMP_UP):
                    continue                   # a wall
                kind = "jump"                  # a low face: jump onto what is above it
            if sj and si and not climb and top_j > max(self.v[q][1] for q in ri["v"]) + STEEP_TOL:
                continue                       # up a slide
            if not sj and hj > hi + STEP_UP:
                if not (self.jumps and hj <= hi + JUMP_UP):
                    continue
                kind = "jump"
            extra = 0.0
            if hi - hj > DROP_MAX:
                extra += DROP_COST
            if (sj or si) and kind != "jump":
                extra += SLIDE_COST
            if climb:
                extra += CLIMB_COST
            if kind == "jump":
                extra += JUMP_COST
            if (rj["flags"] & 0x40) and not (ri["flags"] & 0x40):   # a ceiling below the feet
                cj = self.ceiling(j, mx, mz)
                if cj < hi:
                    continue
            out.append((j, (a, b), extra, kind))
        for j, a, b in self.gaps.get(i, ()):
            if not self.blocked[j]:
                out.append((j, (a, b), JUMP_COST, "gap"))
        return out

    def _gap_links(self):
        """{region: [(far region, a, b)]}: the running jumps over a void (round 19 h5). From a walkable
        region's border edge (no neighbour) to another walkable region's border edge that faces it across
        at most GAP_MAX (XZ), with its floor at most GAP_UP higher and GAP_DROP lower; (a, b) is the take-off
        edge (the portal the funnel passes)."""
        border = []
        for i in range(len(self.r)):
            if not self.walk[i] or self.water[i]:
                continue
            for k, j in enumerate(self.r[i]["neighbours"]):
                if j is None:
                    a, b = self.edge(i, k)
                    border.append((i, a, b))
        cell = 256.0
        grid = {}
        for n, (i, a, b) in enumerate(border):
            key = (int(math.floor((a[0] + b[0]) / 2 / cell)), int(math.floor((a[2] + b[2]) / 2 / cell)))
            grid.setdefault(key, []).append(n)
        out = {}
        for n, (i, a, b) in enumerate(border):
            mx, mz = (a[0] + b[0]) / 2, (a[2] + b[2]) / 2
            ln = math.hypot(b[0] - a[0], b[2] - a[2])
            if ln < 2 * RADIUS:
                continue
            ci = self.cent[i]
            out_side = cross2((a[0], a[2]), (b[0], b[2]), (ci[0], ci[2]))
            hi = self.height(i, mx, mz)
            gx, gz = int(math.floor(mx / cell)), int(math.floor(mz / cell))
            best = {}
            for dx in (-1, 0, 1):
                for dz in (-1, 0, 1):
                    for m in grid.get((gx + dx, gz + dz), ()):
                        j, c, d = border[m]
                        if j == i or j in self.r[i]["neighbours"]:
                            continue
                        nx, nz = (c[0] + d[0]) / 2, (c[2] + d[2]) / 2
                        # the far edge on the outer side of the take-off edge
                        if out_side * cross2((a[0], a[2]), (b[0], b[2]), (nx, nz)) >= 0:
                            continue
                        # the gap: the far edge's nearest point to the take-off edge's midpoint
                        ex, ez = d[0] - c[0], d[2] - c[2]
                        el = ex * ex + ez * ez or 1.0
                        t = max(0.0, min(1.0, ((mx - c[0]) * ex + (mz - c[2]) * ez) / el))
                        px, pz = c[0] + ex * t, c[2] + ez * t
                        gap = math.hypot(px - mx, pz - mz)
                        if gap < 4.0 or gap > GAP_MAX:
                            continue
                        hj = self.height(j, px, pz)
                        if hj > hi + GAP_UP or hj < hi - GAP_DROP:
                            continue
                        if j not in best or gap < best[j][0]:
                            best[j] = (gap, a, b)
            for j, (gap, a2, b2) in best.items():
                out.setdefault(i, []).append((j, a2, b2))
        return out

    def ceiling(self, i, x, z):
        a, b, c = [(p[0], p[3], p[2]) for p in self.corners(i)]
        d = (b[2] - c[2]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[2] - c[2])
        if abs(d) < 1e-9:
            return (a[1] + b[1] + c[1]) / 3
        l1 = ((b[2] - c[2]) * (x - c[0]) + (c[0] - b[0]) * (z - c[2])) / d
        l2 = ((c[2] - a[2]) * (x - c[0]) + (a[0] - c[0]) * (z - c[2])) / d
        return l1 * a[1] + l2 * b[1] + (1 - l1 - l2) * c[1]

    # --- search ---
    def astar(self, s, g):
        """[(region, the link into it or None)] from s to g (A* over region centroids), or None."""
        def hdist(i):
            c, d = self.cent[i], self.cent[g]
            return math.hypot(c[0] - d[0], c[2] - d[2])
        openq = [(hdist(s), 0.0, s)]
        came = {s: None}
        cost = {s: 0.0}
        while openq:
            _, c, i = heapq.heappop(openq)
            if i == g:
                path = []
                while i is not None:
                    path.append((i, came[i][1] if came[i] else None))
                    i = came[i][0] if came[i] else None
                return path[::-1]
            if c > cost.get(i, 1e18):
                continue
            for j, (a, b), extra, kind in self.links(i):
                ci, cj = self.cent[i], self.cent[j]
                m = ((a[0] + b[0]) / 2, (a[2] + b[2]) / 2)
                d = math.hypot(m[0] - ci[0], m[1] - ci[2]) + math.hypot(cj[0] - m[0], cj[2] - m[1])
                if self.water[j]:
                    d *= WATER_COST
                nc = c + d + extra
                if nc < cost.get(j, 1e18):
                    cost[j] = nc
                    came[j] = (i, ((a, b), kind))
                    heapq.heappush(openq, (nc + hdist(j), nc, j))
        return None

    def portals(self, path, start, goal):
        """The edges crossed along a region path [(region, link)] as (left, right) XZ points, shrunk by
        RADIUS; a jump's portal (round 19 h5) is its edge's middle third (he takes off square to it)."""
        out = [((start[0], start[2]), (start[0], start[2]))]
        for (i, _), (j, link) in zip(path, path[1:]):
            (a, b), kind = link
            pa, pb = (a[0], a[2]), (b[0], b[2])
            ci = self.cent[i]
            mid = ((pa[0] + pb[0]) / 2, (pa[1] + pb[1]) / 2)
            if cross2((ci[0], ci[2]), mid, pa) > 0:   # pa to the left of the way in
                left, right = pa, pb
            else:
                left, right = pb, pa
            ln = math.dist(left, right)
            if kind != "walk":
                t = 1.0 / 3.0 if ln > 3 * RADIUS else 0.5
                left, right = (left[0] + (right[0] - left[0]) * t, left[1] + (right[1] - left[1]) * t), \
                              (right[0] + (left[0] - right[0]) * t, right[1] + (left[1] - right[1]) * t)
            elif ln <= 2 * RADIUS:
                left = right = mid
            else:
                t = RADIUS / ln
                left, right = (left[0] + (right[0] - left[0]) * t, left[1] + (right[1] - left[1]) * t), \
                              (right[0] + (left[0] - right[0]) * t, right[1] + (left[1] - right[1]) * t)
            out.append((left, right))
        out.append(((goal[0], goal[2]), (goal[0], goal[2])))
        return out

    @staticmethod
    def funnel(portals):
        """The simple stupid funnel: the shortest XZ polyline through the portals."""
        pts = [portals[0][0]]
        apex = left = right = portals[0][0]
        ai = li = ri = 0
        i = 1
        while i < len(portals):
            pl, pr = portals[i]
            # right side (cross2 < 0 means to the right of apex->x in this convention)
            if cross2(apex, right, pr) >= 0:
                if apex == right or cross2(apex, left, pr) < 0:
                    right, ri = pr, i
                else:
                    pts.append(left)
                    apex, ai = left, li
                    left = right = apex
                    li = ri = ai
                    i = ai + 1
                    continue
            if cross2(apex, left, pl) <= 0:
                if apex == left or cross2(apex, right, pl) > 0:
                    left, li = pl, i
                else:
                    pts.append(right)
                    apex, ai = right, ri
                    left = right = apex
                    li = ri = ai
                    i = ai + 1
                    continue
            i += 1
        if pts[-1] != portals[-1][0]:
            pts.append(portals[-1][0])
        return pts

    def path(self, start, goal):
        """(waypoints [(x, z), ...] from start to goal, region path, length, jumps) or None. jumps (round 19
        h5): [(x, z, kind, (ax, az, bx, bz))] where the waypoint line crosses each jump or gap edge, in order."""
        s = self.locate(*start)
        g = self.locate(*goal)
        if s is None or g is None:
            return None
        rp = self.astar(s, g)
        if rp is None:
            return None
        pts = self.funnel(self.portals(rp, start, goal))
        pts = self.detour(pts, [r for r, _ in rp], start[1], goal[1])
        ln = sum(math.dist(a, b) for a, b in zip(pts, pts[1:]))
        jumps = []
        for r, link in rp[1:]:
            (a, b), kind = link
            if kind == "walk":
                continue
            e0, e1 = (a[0], a[2]), (b[0], b[2])
            hit = None
            for p, q in zip(pts, pts[1:]):
                x = seg_cross(p, q, e0, e1)
                if x is not None:
                    hit = x
                    break
            if hit is None:   # the line passes by an end: the edge's middle
                hit = ((e0[0] + e1[0]) / 2, (e0[1] + e1[1]) / 2)
            jumps.append((hit[0], hit[1], kind, (e0[0], e0[1], e1[0], e1[1])))
        return pts, [r for r, _ in rp], ln, jumps

    def detour(self, pts, rp, y0, y1):
        """Round the static objects' boxes a straight leg crosses: by one or two of the box's corners
        (pushed out by DETOUR_OUT), the shorter way that clears that box. Boxes holding an end are let be."""
        ys = [y0] + [None] * (len(pts) - 2) + [y1]
        out = [pts[0]]
        k = 0
        guard = 0
        while k < len(pts) - 1 and guard < 200:
            guard += 1
            a, b = out[-1], pts[k + 1]
            y = min(y0, y1) if ys[k + 1] is None else ys[k + 1]
            bi = self.seg_box(a, b, y)
            if bi is not None:
                bx = self.boxes[bi]
                if (bx[0] <= a[0] <= bx[1] and bx[2] <= a[1] <= bx[3]) or (bx[0] <= b[0] <= bx[1] and bx[2] <= b[1] <= bx[3]):
                    bi = None
            if bi is None:
                out.append(b)
                k += 1
                continue
            bx = self.boxes[bi]
            o = DETOUR_OUT
            cs = [(bx[0] - o, bx[2] - o), (bx[1] + o, bx[2] - o), (bx[1] + o, bx[3] + o), (bx[0] - o, bx[3] + o)]
            best = None
            for route in [[c] for c in cs] + [[cs[q], cs[(q + 1) % 4]] for q in range(4)] + \
                         [[cs[(q + 1) % 4], cs[q]] for q in range(4)]:
                legs = [a] + route + [b]
                if any(self._seg_hits(legs[m], legs[m + 1], bx) for m in range(len(legs) - 1)):
                    continue
                ln = sum(math.dist(legs[m], legs[m + 1]) for m in range(len(legs) - 1))
                if best is None or ln < best[0]:
                    best = (ln, route)
            if best is None:
                out.append(b)
                k += 1
                continue
            out += best[1]
        return out

    @staticmethod
    def _seg_hits(a, b, bx):
        t0, t1 = 0.0, 1.0
        for (p, d, lo, hi) in ((a[0], b[0] - a[0], bx[0], bx[1]), (a[1], b[1] - a[1], bx[2], bx[3])):
            if abs(d) < 1e-9:
                if not lo < p < hi:
                    return False
                continue
            u0, u1 = (lo - p) / d, (hi - p) / d
            if u0 > u1:
                u0, u1 = u1, u0
            t0, t1 = max(t0, u0), min(t1, u1)
            if t0 >= t1:
                return False
        return True


def main():
    jumps = "--jumps" in sys.argv
    if jumps:
        sys.argv.remove("--jumps")
    rom, level = sys.argv[1], int(sys.argv[2])
    nav = load(rom, level, jumps)
    a = tuple(float(v) for v in sys.argv[3].split(","))
    b = tuple(float(v) for v in sys.argv[4].split(","))
    res = nav.path(a, b)
    if res is None:
        print("no path (regions %s -> %s)" % (nav.locate(*a), nav.locate(*b)))
        return 1
    pts, rp, ln, jumps = res
    print("%d regions, %d waypoints, %.0f units (straight %.0f), %d jumps" % (
        len(rp), len(pts), ln, math.hypot(a[0] - b[0], a[2] - b[2]), len(jumps)))
    for p in pts:
        print("  %.1f %.1f" % p)
    for x, z, kind, _ in jumps:
        print("  %s at %.1f %.1f" % (kind, x, z))
    return 0


if __name__ == "__main__":
    sys.exit(main())
