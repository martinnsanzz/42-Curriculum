# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from src.main.models import DroneMap, Hub, Connection


def hub(name: str, x: int, y: int, line: int = 1) -> Hub:
    return Hub(line=line, name=name, x=x, y=y)


def conn(a: str, b: str, line: int = 1) -> Connection:
    return Connection(line=line, name1=a, name2=b)


def make_drone_map(**overrides: object) -> DroneMap:
    # Fresh objects on every call, never shared between tests
    args: dict[str, object] = {
        "nb_drones": 2,
        "start": hub("start", 0, 0, line=2),
        "end": hub("goal", 2, 0, line=3),
        "hubs": [hub("mid", 1, 0, line=4)],
        "connections": [conn("start", "mid", 5), conn("mid", "goal", 6)],
    }
    args.update(overrides)
    return DroneMap.model_validate(args)


class TestHubNames():
    def test_valid_names(self) -> None:
        drone_map = make_drone_map()
        assert drone_map.start.name == "start"
        assert drone_map.end.name == "goal"
        assert [h.name for h in drone_map.hubs] == ["mid"]

    def test_impossible_goal_is_valid_end(self) -> None:
        drone_map = make_drone_map(
            end=hub("impossible_goal", 2, 0),
            connections=[conn("start", "mid"), conn("mid", "impossible_goal")],
        )
        assert drone_map.end.name == "impossible_goal"

    def test_wrong_start_name(self) -> None:
        with pytest.raises(ValidationError,
                           match="Start hub name must be 'start'"):
            make_drone_map(start=hub("begin", 0, 0))

    def test_wrong_end_name(self) -> None:
        with pytest.raises(ValidationError, match="End hub name must be"):
            make_drone_map(end=hub("finish", 2, 0))

    def test_hub_cannot_be_named_start(self) -> None:
        with pytest.raises(ValidationError,
                           match="Non start_hub can't have name"):
            make_drone_map(hubs=[hub("start", 1, 0)])

    @pytest.mark.parametrize("name", ["goal", "impossible_goal"])
    def test_hub_cannot_use_end_names(self, name: str) -> None:
        with pytest.raises(ValidationError,
                           match="Non end_hub can't have"):
            make_drone_map(hubs=[hub(name, 1, 0)])

    def test_duplicate_hub_names(self) -> None:
        with pytest.raises(ValidationError,
                           match="duplicate name 'mid'"):
            make_drone_map(hubs=[hub("mid", 1, 0), hub("mid", 1, 1)])


class TestCoordinates():
    def test_valid_coordinates(self) -> None:
        drone_map = make_drone_map()
        assert (drone_map.start.x, drone_map.start.y) == (0, 0)
        assert (drone_map.end.x, drone_map.end.y) == (2, 0)
        assert (drone_map.hubs[0].x, drone_map.hubs[0].y) == (1, 0)

    def test_end_start_same_coords(self) -> None:
        with pytest.raises(ValidationError,
                           match="Start and end coordinates can't be "
                           "the same"):
            make_drone_map(start=hub("start", 0, 0), end=hub("goal", 0, 0))

    def test_hub_same_coords_as_start(self) -> None:
        with pytest.raises(ValidationError,
                           match="same coords as start_hub"):
            make_drone_map(hubs=[hub("mid", 0, 0)], connections=[])

    def test_hub_same_coords_as_end(self) -> None:
        with pytest.raises(ValidationError,
                           match="same coords as end_hub"):
            make_drone_map(hubs=[hub("mid", 2, 0)], connections=[])

    def test_two_hubs_same_coords(self) -> None:
        with pytest.raises(ValidationError, match="duplicate coordinates"):
            make_drone_map(hubs=[hub("a", 1, 0), hub("b", 1, 0)],
                           connections=[])

    def test_error_names_the_offending_line(self) -> None:
        with pytest.raises(ValidationError, match="line '9'"):
            make_drone_map(hubs=[hub("a", 1, 0, line=8),
                                 hub("b", 1, 0, line=9)],
                           connections=[])

    def test_same_x_different_y_is_valid(self) -> None:
        drone_map = make_drone_map(
            hubs=[hub("a", 1, 0), hub("b", 1, 1), hub("c", 1, -1)],
            connections=[],
        )
        assert len(drone_map.hubs) == 3

    def test_negative_coordinates_are_valid(self) -> None:
        drone_map = make_drone_map(
            start=hub("start", -5, -5),
            hubs=[hub("mid", -2, 3)],
            connections=[],
        )
        assert drone_map.start.x == -5


class TestConnectionsLink:
    def test_valid_connections(self) -> None:
        drone_map = make_drone_map()
        assert len(drone_map.connections) == 2

    def test_unknown_first_name(self) -> None:
        with pytest.raises(ValidationError,
                           match="non-existing hub name"):
            make_drone_map(connections=[conn("nope", "mid", 7)])

    def test_unknown_second_name(self) -> None:
        with pytest.raises(ValidationError,
                           match="non-existing hub name"):
            make_drone_map(connections=[conn("mid", "nope", 7)])

    def test_unknown_name_reports_line(self) -> None:
        with pytest.raises(ValidationError, match="line '7'"):
            make_drone_map(connections=[conn("mid", "nope", 7)])

    def test_connection_to_start_and_end_is_valid(self) -> None:
        drone_map = make_drone_map(connections=[conn("start", "goal")])
        assert len(drone_map.connections) == 1

    def test_exact_duplicate(self) -> None:
        with pytest.raises(ValidationError, match="duplicate connection"):
            make_drone_map(connections=[conn("start", "mid", 5),
                                        conn("start", "mid", 6)])

    def test_reversed_duplicate(self) -> None:
        with pytest.raises(ValidationError, match="duplicate connection"):
            make_drone_map(connections=[conn("start", "mid", 5),
                                        conn("mid", "start", 6)])

    def test_duplicate_reports_both_lines(self) -> None:
        with pytest.raises(ValidationError,
                           match="line 6.\n- Same as line '5'"):
            make_drone_map(connections=[conn("start", "mid", 5),
                                        conn("mid", "start", 6)])

    def test_duplicate_later_in_list(self) -> None:
        with pytest.raises(ValidationError, match="duplicate connection"):
            make_drone_map(connections=[conn("start", "mid", 5),
                                        conn("mid", "goal", 6),
                                        conn("goal", "mid", 7)])

    def test_different_pairs_are_not_duplicates(self) -> None:
        drone_map = make_drone_map(connections=[conn("start", "mid"),
                                                conn("start", "goal"),
                                                conn("mid", "goal")])
        assert len(drone_map.connections) == 3

    def test_self_connection(self) -> None:
        with pytest.raises(ValidationError, match="connects a hub to itself"):
            make_drone_map(connections=[conn("mid", "mid")])
