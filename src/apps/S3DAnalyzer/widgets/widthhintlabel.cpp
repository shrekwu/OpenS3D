#include "widthhintlabel.h"

#include <gsl/gsl_util>

WidthHintLabel::WidthHintLabel(QWidget* parent) : QLabel(parent) {
  initPixmaps();

  setAlignment(Qt::AlignCenter);
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

  auto&& pixmap = m_pixmaps[static_cast<int>(State::Neutral)];
  setMinimumSize(pixmap->width() + 5, pixmap->height() + 4);
}

void WidthHintLabel::setState(State state) {
  setPixmap(*gsl::at(m_pixmaps, static_cast<int>(state)));
  update();
}

void WidthHintLabel::updateHint(float ratio) {
  // ok this is the worst
  // todo: think a bit more about this
  // it's way worse to have more than not enough range
  if (inRange(0.8, 1.0, ratio)) {
    setState(State::Neutral);
  } else if (inRange(0.7, 0.8, ratio)) {
    setState(State::ShouldWiden);
  } else if (inRange(1.0, 1.05, ratio)) {
    setState(State::ShouldNarrow);
  } else if (inRange(0.6, 0.7, ratio)) {
    setState(State::ShouldWidenMuch);
  } else if (inRange(1.05, 1.1, ratio)) {
    setState(State::ShouldNarrowMuch);
  } else if (ratio < 0.6) {
    setState(State::ShouldWidenVeryMuch);
  } else if (ratio >= 1.1) {
    setState(State::ShouldNarrowVeryMuch);
  }
  update();
}

// todo, this should be outside
bool WidthHintLabel::inRange(float min, float max, float value) {
  return min <= value && value < max;
}

void WidthHintLabel::initPixmaps() {
  initPixmap(State::ShouldNarrowVeryMuch, ":icons/in_arrows_red.png");
  initPixmap(State::ShouldNarrowMuch, ":icons/in_arrows_orange.png");
  initPixmap(State::ShouldNarrow, ":icons/in_arrows_green.png");
  initPixmap(State::ShouldWidenVeryMuch, ":icons/out_arrows_red.png");
  initPixmap(State::ShouldWidenMuch, ":icons/out_arrows_orange.png");
  initPixmap(State::ShouldWiden, ":icons/out_arrows_green.png");
  initPixmap(State::Neutral, ":icons/neutral_arrows.png");

  setPixmap(*m_pixmaps[static_cast<int>(State::Neutral)]);
}

void WidthHintLabel::initPixmap(State state, const QString& filepath) {
  auto pixmap = std::make_unique<QPixmap>(filepath);
  gsl::at(m_pixmaps, static_cast<int>(state)) = std::move(pixmap);
}
