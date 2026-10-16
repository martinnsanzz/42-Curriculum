# Built-in modules
from typing import Any

# Installed modules
import pytest

# Local modules
from main.backend.graph import ZONE_COST, Graph
from main.exceptions import GraphError
from main.models import (Connection, ConnectionMetadata, DroneMap, Hub,
                         HubMetadata, Zone)

NAMES = ["start", "a", "b", "c", "x", "goal", "island"]


def hub(name: str, x: int, y: int, line: int = 1, **meta: Any) -> Hub:
    return Hub(line=line, name=name, x=x, y=y, metadata=HubMetadata(**meta))


def conn(a: str, b: str, line: int = 1, cap: int = 1) -> Connection:
    return Connection(line=line, name1=a, name2=b,
                      metadata=ConnectionMetadata(max_link_capacity=cap))


@pytest.fixture
def drone_map() -> DroneMap:
    # start - a - b(restricted) - goal
    #          \- c(priority) - x(blocked) - goal      island: no links
    return DroneMap(
        nb_drones=3,
        start=hub("start", 0, 0, line=2),
        end=hub("goal", 3, 0, line=3),
        hubs=[
            hub("a", 1, 0, line=4),
            hub("b", 2, 0, line=5, zone=Zone.RESTRICTED, max_drones=2),
            hub("c", 1, 1, line=6, zone=Zone.PRIORITY),
            hub("x", 2, 1, line=7, zone=Zone.BLOCKED),
            hub("island", 5, 5, line=8, max_drones=4),
        ],
        connections=[
            conn("start", "a", 9, cap=2),
            conn("a", "b", 10),
            conn("b", "goal", 11),
            conn("a", "c", 12),
            conn("c", "x", 13),
            conn("x", "goal", 14),
        ],
    )


@pytest.fixture
def graph(drone_map: DroneMap) -> Graph:
    return Graph.from_drone_map(drone_map)


class TestFromDroneMap:
    def test_start_and_end(self, graph: Graph) -> None:
        assert graph.start == "start"
        assert graph.end == "goal"

    def test_every_zone_is_present(self, graph: Graph) -> None:
        for name in NAMES:
            graph.all_neighbours(name)   # raises KeyError if missing

    def test_constructor_stores_start_and_end(self) -> None:
        h = {"s": hub("s", 0, 0), "e": hub("e", 1, 0)}
        g = Graph(5, h, {"s": ["e"], "e": ["s"]}, {}, "s", "e")
        assert (g.start, g.end) == ("s", "e")


class TestAdjacency:
    def test_expected_neighbours(self, graph: Graph) -> None:
        assert sorted(graph.all_neighbours("start")) == ["a"]
        assert sorted(graph.all_neighbours("a")) == ["b", "c", "start"]
        assert sorted(graph.all_neighbours("goal")) == ["b", "x"]

    def test_is_symmetric(self, graph: Graph) -> None:
        for name in NAMES:
            for other in graph.all_neighbours(name):
                assert name in graph.all_neighbours(other)

    def test_isolated_zone_has_empty_list(self, graph: Graph) -> None:
        assert graph.all_neighbours("island") == []
        assert graph.valid_neighbours("island") == []

    def test_connections_to_unknown_zones_are_skipped(self) -> None:
        hubs = {"a": hub("a", 0, 0), "b": hub("b", 1, 0)}
        adjacency = Graph._build_adjacency(
            hubs, [conn("a", "b"), conn("a", "ghost")])
        assert adjacency == {"a": ["b"], "b": ["a"]}


class TestValidNeighbours:
    def test_blocked_zone_is_excluded(self, graph: Graph) -> None:
        assert sorted(graph.valid_neighbours("goal")) == ["b"]
        assert sorted(graph.valid_neighbours("c")) == ["a"]

    def test_does_not_change_the_graph(self, graph: Graph) -> None:
        before = list(graph.all_neighbours("c"))
        graph.valid_neighbours("c")
        graph.valid_neighbours("c")
        assert graph.all_neighbours("c") == before
        assert "x" in graph.all_neighbours("c")

    def test_result_is_a_new_list(self, graph: Graph) -> None:
        result = graph.valid_neighbours("a")
        result.append("zzz")
        assert "zzz" not in graph.valid_neighbours("a")

    @pytest.mark.parametrize("name", NAMES)
    def test_never_returns_a_zone_cost_rejects(self, graph: Graph,
                                               name: str) -> None:
        for n in graph.valid_neighbours(name):
            graph.cost(n)   # would raise GraphError for a blocked zone

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.valid_neighbours("nope")


class TestAllNeighbours:
    def test_includes_blocked_zones(self, graph: Graph) -> None:
        assert "x" in graph.all_neighbours("c")
        assert "x" in graph.all_neighbours("goal")

    def test_returns_a_copy(self, graph: Graph) -> None:
        result = graph.all_neighbours("a")
        result.append("zzz")
        assert "zzz" not in graph.all_neighbours("a")

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.all_neighbours("nope")


class TestIsBlocked:
    def test_blocked(self, graph: Graph) -> None:
        assert graph.is_blocked("x") is True

    @pytest.mark.parametrize("name", ["start", "a", "b", "c", "goal"])
    def test_not_blocked(self, graph: Graph, name: str) -> None:
        assert graph.is_blocked(name) is False

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.is_blocked("nope")


class TestCost:
    def test_normal(self, graph: Graph) -> None:
        assert graph.cost("a") == 1

    def test_priority(self, graph: Graph) -> None:
        assert graph.cost("c") == 1

    def test_restricted(self, graph: Graph) -> None:
        assert graph.cost("b") == 2

    def test_blocked_raises_and_names_the_zone(self, graph: Graph) -> None:
        with pytest.raises(GraphError, match="blocked zone 'x'"):
            graph.cost("x")

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.cost("nope")

    def test_every_enterable_zone_type_has_a_cost(self) -> None:
        for zone in Zone:
            if zone != Zone.BLOCKED:
                assert zone in ZONE_COST
        assert Zone.BLOCKED not in ZONE_COST

class TestCheckHubPriority:
    def test_priority_zone_is_negative_one(self, graph: Graph) -> None:
        assert graph.check_hub_priority("c") == -1

    @pytest.mark.parametrize("name", ["start", "a", "b", "x", "goal", "island"])
    def test_other_zones_are_zero(self, graph: Graph, name: str) -> None:
        assert graph.check_hub_priority(name) == 0

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.check_hub_priority("nope")

class TestZoneCapacity:
    def test_default_is_one(self, graph: Graph) -> None:
        assert graph.zone_capacity("a") == 1

    def test_explicit_values(self, graph: Graph) -> None:
        assert graph.zone_capacity("b") == 2
        assert graph.zone_capacity("island") == 4

    def test_start_and_end_hold_every_drone(self, graph: Graph) -> None:
        assert graph.zone_capacity("start") == 3
        assert graph.zone_capacity("goal") == 3

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(KeyError):
            graph.zone_capacity("nope")


class TestLinkCapacity:
    def test_explicit_value(self, graph: Graph) -> None:
        assert graph.link_capacity("start", "a") == 2

    def test_default_is_one(self, graph: Graph) -> None:
        assert graph.link_capacity("a", "b") == 1

    def test_is_symmetric(self, graph: Graph) -> None:
        assert graph.link_capacity("a", "start") == 2
        assert (graph.link_capacity("a", "b")
                == graph.link_capacity("b", "a"))

    def test_link_to_blocked_zone_still_exists(self, graph: Graph) -> None:
        assert graph.link_capacity("c", "x") == 1

    def test_unconnected_zones(self, graph: Graph) -> None:
        with pytest.raises(GraphError, match="No link found between"):
            graph.link_capacity("start", "goal")

    def test_unknown_zone(self, graph: Graph) -> None:
        with pytest.raises(GraphError, match="Unknown zone 'nope'"):
            graph.link_capacity("a", "nope")
        with pytest.raises(GraphError, match="Unknown zone 'nope'"):
            graph.link_capacity("nope", "a")

    def test_self_pair(self, graph: Graph) -> None:
        with pytest.raises(GraphError, match="No link found"):
            graph.link_capacity("a", "a")

    def test_one_entry_per_connection(self) -> None:
        caps = Graph._build_link_caps([conn("a", "b"), conn("b", "c", cap=3)])
        assert caps == {frozenset({"a", "b"}): 1, frozenset({"b", "c"}): 3}