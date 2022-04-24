#pragma once

class BoardState {
 public:
  const bool _white_move;
  const bool _has_ep_pawn;
  const bool _white_has_long_castle;
  const bool _white_has_short_castle;
  const bool _black_has_long_castle;
  const bool _black_has_short_castle;

  constexpr BoardState(bool white_move, bool has_ep_pawn,
                       bool white_has_long_castle, bool white_has_short_castle,
                       bool black_has_long_castle, bool black_has_short_castle)
      : _white_move(white_move),
        _has_ep_pawn(has_ep_pawn),
        _white_has_long_castle(white_has_long_castle),
        _white_has_short_castle(white_has_short_castle),
        _black_has_long_castle(white_has_long_castle),
        _black_has_short_castle(black_has_short_castle) {}

  inline constexpr bool has_long_castle() const {
    if (_white_move)
      return _white_has_long_castle;
    else
      return _black_has_long_castle;
  }

  inline constexpr bool has_short_castle() const {
    if (_white_move)
      return _white_has_short_castle;
    else
      return _black_has_short_castle;
  }

  inline constexpr BoardState register_short_rook_move() const {
    if (_white_move) {
      return BoardState(!_white_move, false, _white_has_long_castle, false,
                        _black_has_long_castle, _black_has_short_castle);
    } else {
      return BoardState(!_white_move, false, _white_has_long_castle,
                        _white_has_short_castle, _black_has_long_castle, false);
    }
  }

  inline constexpr BoardState register_long_rook_move() const {
    if (_white_move) {
      return BoardState(!_white_move, false, false, _white_has_short_castle,
                        _black_has_long_castle, _black_has_short_castle);
    } else {
      return BoardState(!_white_move, false, _white_has_long_castle,
                        _white_has_short_castle, false,
                        _black_has_short_castle);
    }
  }

  inline constexpr BoardState register_king_move() const {
    if (_white_move) {
      return BoardState(!_white_move, false, false, false,
                        _black_has_long_castle, _black_has_short_castle);
    } else {
      return BoardState(!_white_move, false, _white_has_long_castle,
                        _white_has_short_castle, false, false);
    }
  }

  inline constexpr BoardState register_move() const {
    return BoardState(!_white_move, false, _white_has_long_castle,
                      _white_has_short_castle, _black_has_long_castle,
                      _black_has_short_castle);
  }
};
