# Built-in modules
from typing import Any

# Installed modules
import pytest

# Local modules
from src.main.backend.graph import Graph
from src.main.backend.pathfinder import PathFinder
from src.main.exceptions import GraphError
from src.main.models import (Connection, ConnectionMetadata, DroneMap, Hub,
                             HubMetadata, HubType)


def make_graph(hubs: dict[str, dict[str, Any]],
               links: list[tuple[str, str]],
               end: str = "goal",
               end_meta: dict[str, Any] | None = None) -> PathFinder:
    """Build a PathFinder from zone names (with metadata) and links.

    Start is always 'start' and the end is 'goal' (or `end`). Coordinates
    are generated, so they never clash.
    """
    hub_models = [
        Hub(line=i + 3, name=name, x=i + 1, y=1,
            metadata=HubMetadata(**meta))
        for i, (name, meta) in enumerate(hubs.items())
    ]
    drone_map = DroneMap(
        nb_drones=2,
        start=Hub(line=2, name="start", x=0, y=0, metadata=HubMetadata()),
        end=Hub(line=1, name=end, x=100, y=0,
                metadata=HubMetadata(**(end_meta or {}))),
        hubs=hub_models,
        connections=[
            Connection(line=100 + i, name1=a, name2=b,
                       metadata=ConnectionMetadata())
            for i, (a, b) in enumerate(links)
        ],
    )
    return PathFinder(Graph.from_drone_map(drone_map))


RESTRICTED: dict[str, Any] = {"hub_type": HubType.RESTRICTED}
BLOCKED: dict[str, Any] = {"hub_type": HubType.BLOCKED}


class TestComputeDistance:
    def test_linear_map(self) -> None:
        pf = make_graph({"a": {}, "b": {}},
                        [("start", "a"), ("a", "b"), ("b", "goal")])
        assert pf.compute_distance() == {"goal": 0, "b": 1, "a": 2,
                                         "start": 3}

    def test_restricted_zone_in_the_middle(self) -> None:
        pf = make_graph({"a": RESTRICTED}, [("start", "a"), ("a", "goal")])
        distance = pf.compute_distance()
        assert distance["a"] == 1       # a -> goal: enter goal (1)
        assert distance["start"] == 3   # start -> a (2) -> goal (1)

    def test_restricted_end_costs_two_to_enter(self) -> None:
        pf = make_graph({"a": {}}, [("start", "a"), ("a", "goal")],
                        end_meta=RESTRICTED)
        distance = pf.compute_distance()
        assert distance["a"] == 2       # entering the goal costs 2
        assert distance["start"] == 3

    def test_zone_found_by_a_longer_route_first_is_improved(self) -> None:
        # 'a_r' is popped before 'z_n', so 'u' is first reached through the
        # restricted zone (3) and must then be lowered to 2.
        pf = make_graph(
            {"a_r": RESTRICTED, "z_n": {}, "u": {}},
            [("goal", "a_r"), ("goal", "z_n"), ("a_r", "u"),
             ("z_n", "u"), ("start", "u")],
        )
        distance = pf.compute_distance()
        assert distance["u"] == 2
        assert distance["start"] == 3

    def test_blocked_zone_never_appears(self) -> None:
        pf = make_graph({"a": {}, "x": BLOCKED},
                        [("start", "a"), ("a", "goal"),
                         ("start", "x"), ("x", "goal")])
        assert pf.compute_distance() == {"goal": 0, "a": 1, "start": 2}

    def test_zones_behind_a_blocked_zone_are_absent(self) -> None:
        pf = make_graph({"a": {}, "x": BLOCKED, "y": {}},
                        [("start", "a"), ("a", "goal"),
                         ("goal", "x"), ("x", "y")])
        assert "y" not in pf.compute_distance()

    def test_disconnected_zone_is_absent(self) -> None:
        pf = make_graph({"a": {}, "island": {}},
                        [("start", "a"), ("a", "goal")])
        assert "island" not in pf.compute_distance()

    def test_dead_end_does_not_change_the_main_route(self) -> None:
        pf = make_graph({"a": {}, "d": {}},
                        [("start", "a"), ("a", "d"), ("a", "goal")])
        distance = pf.compute_distance()
        assert distance["start"] == 2
        assert distance["d"] == 2

    def test_start_unreachable(self) -> None:
        pf = make_graph({"a": {}}, [("a", "goal")])
        with pytest.raises(GraphError, match="Starting point is unreachable"):
            pf.compute_distance()

    def test_only_route_is_blocked(self) -> None:
        pf = make_graph({"x": BLOCKED}, [("start", "x"), ("x", "goal")])
        with pytest.raises(GraphError, match="unreachable"):
            pf.compute_distance()

    def test_blocked_end(self) -> None:
        pf = make_graph({"a": {}}, [("start", "a"), ("a", "goal")],
                        end_meta=BLOCKED)
        with pytest.raises(GraphError, match="blocked hub"):
            pf.compute_distance()

    def test_same_result_every_call(self) -> None:
        pf = make_graph({"a": {}}, [("start", "a"), ("a", "goal")])
        assert pf.compute_distance() == pf.compute_distance()
