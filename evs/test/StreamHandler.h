/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAR_EVS_APP_STREAMHANDLER_H
#define CAR_EVS_APP_STREAMHANDLER_H

#include <android/hardware/evs/1.0/IEvsCameraStream.h>
#include <android/hardware/evs/1.0/IEvsCamera.h>
#include <android/hardware/evs/1.0/IEvsDisplay.h>

using namespace ::android::hardware::evs::V1_0;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_handle;
using ::android::sp;


class StreamHandler : public IEvsCameraStream {
public:
    StreamHandler(android::sp <IEvsCamera>  pCamera,  CameraDesc  cameraInfo,
                  android::sp <IEvsDisplay> pDisplay, DisplayDesc displayInfo);

    void startStream();
    void asyncStopStream();
    void blockingStopStream();

    bool isRunning();

    unsigned getFramesReceived();
    unsigned getFramesCompleted();

private:
    // Implementation for ::android::hardware::evs::V1_0::ICarCameraStream
    Return<void> deliverFrame(const BufferDesc& buffer)  override;

    // Local implementation details
    bool copyBufferContents(const BufferDesc& tgtBuffer, const BufferDesc& srcBuffer);
    void registerBufferHelper(const BufferDesc& buffer);
    void unregisterBufferHelper(const BufferDesc& buffer);

    android::sp <IEvsCamera>    mCamera;
    CameraDesc                  mCameraInfo;
    android::sp <IEvsDisplay>   mDisplay;
    DisplayDesc                 mDisplayInfo;

    std::mutex                  mLock;
    std::condition_variable     mSignal;

    bool                        mRunning = false;

    unsigned                    mFramesReceived = 0;    // Simple counter -- rolls over eventually!
    unsigned                    mFramesCompleted = 0;   // Simple counter -- rolls over eventually!
};


#endif //CAR_EVS_APP_STREAMHANDLER_H