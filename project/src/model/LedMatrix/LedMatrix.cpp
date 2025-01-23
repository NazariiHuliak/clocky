#include "LedMatrix.h"

LedMatrix::LedMatrix() {}

LedMatrix::~LedMatrix() {}

void LedMatrix::setWatchFace(WatchFace* watchface, boolean modeWasChanged) {
    watchface->setFrame(modeWasChanged);
}