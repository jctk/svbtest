#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

#include "SVBCameraSDK.h"

class SVBAPIERR {
public:
    SVBAPIERR(SVB_ERROR_CODE err, const char* msg) { m_err = err; m_msg = strdup(msg); }
    ~SVBAPIERR() { free(m_msg); }
    void puterr() { fprintf(stderr, "error:%d:%s\n", m_err, m_msg); }
private:
    SVB_ERROR_CODE m_err;
    char* m_msg;
};

int main(int argc, char** argv)
{
    /*
        chech argument
    */
    if (argc != 2) {
        fputs("A device name must be specified which SV405CC or SV905C.\n", stderr);
        return -1;
    }
    char* fname = strcat(strdup("SVBONY "), argv[1]); // generate Friendly Name

    printf("SVBONY Camera SDK Version: %s\n",SVBGetSDKVersion());

    try {
        SVB_ERROR_CODE ret;
        SVB_CAMERA_INFO CameraInfo;   
        long exposure = 1;

        // Seek specified device
        int nCameras = SVBGetNumOfConnectedCameras();
        int iCameraID = -1;
        for (int i = 0; i < nCameras; i++) {
            if (SVB_SUCCESS != (ret = SVBGetCameraInfo(&CameraInfo, i))) {
                throw SVBAPIERR(ret, "SVBGetCameraInfo");
            }
            printf("#%d %s\n", i, CameraInfo.FriendlyName);
            if (!strcmp(fname, CameraInfo.FriendlyName)) {
                iCameraID = CameraInfo.CameraID;
            }
        }
        if (iCameraID == -1) {
            fprintf(stderr, "Cannot find device '%s'\n", fname);
            throw -1;
        }
        else {
            printf("Found device '%s'\n", fname);
        }

        if (SVB_SUCCESS != (ret = SVBOpenCamera(iCameraID)))
            throw SVBAPIERR(ret, "SVBOpenCamera");

        SVBSetAutoSaveParam(iCameraID, SVB_FALSE);

        SVB_CAMERA_PROPERTY CameraProperty;
        if (SVB_SUCCESS != (ret = SVBGetCameraProperty(iCameraID, &CameraProperty)))
            throw SVBAPIERR(ret, "SVBGetCameraProperty");

        SVB_CAMERA_PROPERTY_EX CameraPropertyEx;
        if (SVB_SUCCESS != (ret = SVBGetCameraPropertyEx(iCameraID, &CameraPropertyEx)))
            throw SVBAPIERR(ret, "SVBGetCameraPropertyEx");

        SVB_SUPPORTED_MODE CameraSupportMode;
        if (SVB_SUCCESS != (ret = SVBGetCameraSupportMode(iCameraID, &CameraSupportMode)))
            throw SVBAPIERR(ret, "SVBGetCameraSupportMode");

        int controlsNum;
        if (SVB_SUCCESS != (ret = SVBGetNumOfControls(iCameraID, &controlsNum)))
           throw SVBAPIERR(ret, "");

        // fix for SDK gain error issue. Set exposure time
        if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_EXPOSURE, -1000000, SVB_FALSE)))
           throw SVBAPIERR(ret, "");

        for(int i = 0; i < controlsNum; i++)
        {
            // read control
            SVB_CONTROL_CAPS caps;
            if (SVB_SUCCESS != (ret = SVBGetControlCaps(iCameraID, i, &caps)))
                throw SVBAPIERR(ret, "");

            switch(caps.ControlType) {
                case SVB_EXPOSURE :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_EXPOSURE, exposure * 1000000L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");
                    break;

                case SVB_GAIN :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_GAIN, 10, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_CONTRAST :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_CONTRAST, 50L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");
                    break;

                case SVB_SHARPNESS :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_SHARPNESS, 0L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_SATURATION :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_SATURATION, 100L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_WB_R :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_WB_R, 100L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_WB_G :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_WB_G, 100L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_WB_B :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_WB_B, 100L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");

                    break;

                case SVB_GAMMA :
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_GAMMA, 100L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");
                    break;

                case SVB_BLACK_LEVEL :
                    // Dark Offset
                    if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_BLACK_LEVEL, 0L, SVB_FALSE)))
                        throw SVBAPIERR(ret, "");
                    break;

                default :
                    break;
            }
        }

        if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_GAMMA_CONTRAST, 50L, SVB_FALSE)))
            throw SVBAPIERR(ret, "");

        if (SVB_SUCCESS != (ret = SVBSetControlValue(iCameraID, SVB_FLIP, 0, SVB_FALSE)))
            throw SVBAPIERR(ret, "");

        /*
            test exposure
        */

        if (SVB_SUCCESS != (ret = SVBStopVideoCapture(iCameraID)))
            throw SVBAPIERR(ret, "");

        if (SVB_SUCCESS != (ret = SVBSetOutputImageType(iCameraID, SVB_IMG_RAW16)))
            throw SVBAPIERR(ret, "");

        struct ROI_t {
            int x, y, w, h;
        } rois[] ={
            { 0, 0, (int)CameraProperty.MaxWidth, (int)CameraProperty.MaxHeight },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 },
            { 0, 0, 192, 192 }            
        };
        unsigned char* buffer = NULL;
        for (int j = 0; j < sizeof(rois)/sizeof(rois[0]); j++) {
            printf("#%d\n", j);
            if (SVB_SUCCESS != (ret = SVBSetROIFormat(iCameraID, rois[j].x, rois[j].y, rois[j].w, rois[j].h, 1)))
                throw SVBAPIERR(ret, "");

            int x, y, w, h, b;
            if (SVB_SUCCESS != (ret = SVBGetROIFormat(iCameraID, &x, &y, &w, &h, &b)))
                throw SVBAPIERR(ret, "");
            printf("ROI(%d, %d, %d, %d, %d)\n", x, y, w, h, b);

            // Allocate image buffer
            long buffer_size = (CameraProperty.MaxBitDepth + 7) / 8 * w*h*4;
            buffer = (unsigned char*)realloc(buffer, buffer_size);
            memset(buffer, 0, buffer_size);

            if (SVB_SUCCESS != (ret = SVBStartVideoCapture(iCameraID)))
                throw SVBAPIERR(ret, "");

            if (SVB_SUCCESS != (ret = SVBSetCameraMode(iCameraID, SVB_MODE_TRIG_SOFT)))
                throw SVBAPIERR(ret, "");

            if (SVB_SUCCESS != (ret = SVBSendSoftTrigger(iCameraID)))
                throw SVBAPIERR(ret, "");

            for (int i = 0; i < exposure; i++) {
                printf("left %d sec\n", exposure-i);
                usleep(1000000); // Wait duaring exposuer
            }

            puts("begin:SVBGetVideoData");
            int tried = 0;
            do {
                ret = SVBGetVideoData(iCameraID, buffer, buffer_size, 1000);
                printf("#%d:SVBGetVideoData:%d\n", ++tried, ret);
            } while (ret != SVB_SUCCESS);

            if (SVB_SUCCESS != (ret = SVBStopVideoCapture(iCameraID)))
                throw SVBAPIERR(ret, "");

            // write captured image.
            char fname[255];
            sprintf(fname, "./raw-%d.img", j);
            FILE *fp = fopen(fname, "w");
            fwrite(buffer, buffer_size, 1, fp);
            fclose(fp);
        }

        if (SVB_SUCCESS != (ret = SVBCloseCamera(iCameraID)))
            throw SVBAPIERR(ret, "");

        /*
            test cooling
        */
       // *** Not yet implemented. ***
    }
    catch (SVBAPIERR& SVBAPIERR) {
        SVBAPIERR.puterr();
        free(fname);
        return -1;
    }
    catch (int err) {
        free(fname);
        return err;
    }
    return 0;
}