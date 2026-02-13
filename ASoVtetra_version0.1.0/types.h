#pragma once

#include <mutex>

// ------------------------------
// 表情データ
// ------------------------------
struct FaceParams {
    float jawOpen;
    float eyeBlinkLeft;
    float eyeBlinkRight;
    float yaw;
    float browL;
    float browR;
};

// ------------------------------
// 口の状態
// ------------------------------
enum class MouthState { Closed, Open };

// ------------------------------
// Transform 設定
// ------------------------------
struct TransformSettings {
    float earMoveScale;
    float browUpY;
    float browUpRotL;
    float browUpRotR;
    float earringTopRotScale;
    float earringBottomRotScale;
    float faceMoveScale;

    float pivotEarringX;
    float pivotEarringY;

    float pivotBrowLX;
    float pivotBrowLY;

    float pivotBrowRX;
    float pivotBrowRY;

    float pivotEarX;
    float pivotEarY;
};