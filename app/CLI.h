#include <cstdlib>
#if defined(USE_EXPERIMENTAL_FS)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#if defined(__APPLE__)
#include <unistd.h>
#endif
#endif
#include <cstdint>
#include <iomanip>
#include <thread>
#include <chrono>
#include <iostream>
#include "CRSDK/CameraRemote_SDK.h"
#include "CameraDevice.h"
#include "Text.h"
#include "clipp.h"

enum class mode {
    capture,
    get,
    set,
    sdk,
    help
};

//#define LIVEVIEW_ENB

namespace SDK = SCRSDK;
using namespace SCRSDK;
using namespace cli;
using namespace std::chrono_literals;
using namespace clipp;
using std::string;

typedef std::shared_ptr<CameraDevice> CameraDevicePtr;

const std::unordered_map<text, CrInt32u> map_device_property
{
    {TEXT("Undefined"), CrDeviceProperty_Undefined},
    {TEXT("S1"), CrDeviceProperty_S1},
    {TEXT("AEL"), CrDeviceProperty_AEL},
    {TEXT("FEL"), CrDeviceProperty_FEL},
    {TEXT("AFL"), CrDeviceProperty_AFL},
    {TEXT("AWBL"), CrDeviceProperty_AWBL},
    {TEXT("FNumber"), CrDeviceProperty_FNumber},
    {TEXT("ExposureBiasCompensation"), CrDeviceProperty_ExposureBiasCompensation},
    {TEXT("FlashCompensation"), CrDeviceProperty_FlashCompensation},
    {TEXT("ShutterSpeed"), CrDeviceProperty_ShutterSpeed},
    {TEXT("IsoSensitivity"), CrDeviceProperty_IsoSensitivity},
    {TEXT("ExposureProgramMode"), CrDeviceProperty_ExposureProgramMode},
    {TEXT("FileType"), CrDeviceProperty_FileType},
    {TEXT("JpegQuality"), CrDeviceProperty_JpegQuality},
    {TEXT("WhiteBalance"), CrDeviceProperty_WhiteBalance},
    {TEXT("FocusMode"), CrDeviceProperty_FocusMode},
    {TEXT("MeteringMode"), CrDeviceProperty_MeteringMode},
    {TEXT("FlashMode"), CrDeviceProperty_FlashMode},
    {TEXT("WirelessFlash"), CrDeviceProperty_WirelessFlash},
    {TEXT("RedEyeReduction"), CrDeviceProperty_RedEyeReduction},
    {TEXT("DriveMode"), CrDeviceProperty_DriveMode},
    {TEXT("DRO"), CrDeviceProperty_DRO},
    {TEXT("ImageSize"), CrDeviceProperty_ImageSize},
    {TEXT("AspectRatio"), CrDeviceProperty_AspectRatio},
    {TEXT("PictureEffect"), CrDeviceProperty_PictureEffect},
    {TEXT("FocusArea"), CrDeviceProperty_FocusArea},
    {TEXT("Colortemp"), CrDeviceProperty_Colortemp},
    {TEXT("ColorTuningAB"), CrDeviceProperty_ColorTuningAB},
    {TEXT("ColorTuningGM"), CrDeviceProperty_ColorTuningGM},
    {TEXT("LiveViewDisplayEffect"), CrDeviceProperty_LiveViewDisplayEffect},
    {TEXT("StillImageStoreDestination"), CrDeviceProperty_StillImageStoreDestination},
    {TEXT("PriorityKeySettings"), CrDeviceProperty_PriorityKeySettings},
    {TEXT("Focus_Magnifier_Setting"), CrDeviceProperty_Focus_Magnifier_Setting},
    {TEXT("DateTime_Settings"), CrDeviceProperty_DateTime_Settings},
    {TEXT("NearFar"), CrDeviceProperty_NearFar},
    {TEXT("AF_Area_Position"), CrDeviceProperty_AF_Area_Position},
    {TEXT("Zoom_Scale"), CrDeviceProperty_Zoom_Scale},
    {TEXT("Zoom_Setting"), CrDeviceProperty_Zoom_Setting},
    {TEXT("Zoom_Operation"), CrDeviceProperty_Zoom_Operation},
    {TEXT("Movie_File_Format"), CrDeviceProperty_Movie_File_Format},
    {TEXT("Movie_Recording_Setting"), CrDeviceProperty_Movie_Recording_Setting},
    {TEXT("Movie_Recording_FrameRateSetting"), CrDeviceProperty_Movie_Recording_FrameRateSetting},
    {TEXT("CompressionFileFormatStill"), CrDeviceProperty_CompressionFileFormatStill},
    {TEXT("MediaSLOT1_FileType"), CrDeviceProperty_MediaSLOT1_FileType},
    {TEXT("MediaSLOT2_FileType"), CrDeviceProperty_MediaSLOT2_FileType},
    {TEXT("MediaSLOT1_JpegQuality"), CrDeviceProperty_MediaSLOT1_JpegQuality},
    {TEXT("MediaSLOT2_JpegQuality"), CrDeviceProperty_MediaSLOT2_JpegQuality},
    {TEXT("MediaSLOT1_ImageSize"), CrDeviceProperty_MediaSLOT1_ImageSize},
    {TEXT("MediaSLOT2_ImageSize"), CrDeviceProperty_MediaSLOT2_ImageSize},
    {TEXT("RAW_FileCompressionType"), CrDeviceProperty_RAW_FileCompressionType},
    {TEXT("MediaSLOT1_RAW_FileCompressionType"), CrDeviceProperty_MediaSLOT1_RAW_FileCompressionType},
    {TEXT("MediaSLOT2_RAW_FileCompressionType"), CrDeviceProperty_MediaSLOT2_RAW_FileCompressionType},
    {TEXT("ZoomAndFocusPosition_Save"), CrDeviceProperty_ZoomAndFocusPosition_Save},
    {TEXT("ZoomAndFocusPosition_Load"), CrDeviceProperty_ZoomAndFocusPosition_Load},
    {TEXT("S2"), CrDeviceProperty_S2},
    {TEXT("Interval_Rec_Mode"), CrDeviceProperty_Interval_Rec_Mode},
    {TEXT("Still_Image_Trans_Size"), CrDeviceProperty_Still_Image_Trans_Size},
    {TEXT("RAW_J_PC_Save_Image"), CrDeviceProperty_RAW_J_PC_Save_Image},
    {TEXT("LiveView_Image_Quality"), CrDeviceProperty_LiveView_Image_Quality},
    {TEXT("CustomWB_Capture_Standby"), CrDeviceProperty_CustomWB_Capture_Standby},
    {TEXT("CustomWB_Capture_Standby_Cancel"), CrDeviceProperty_CustomWB_Capture_Standby_Cancel},
    {TEXT("CustomWB_Capture"), CrDeviceProperty_CustomWB_Capture},
    {TEXT("Remocon_Zoom_Speed_Type"), CrDeviceProperty_Remocon_Zoom_Speed_Type},
    {TEXT("GetOnly"), CrDeviceProperty_GetOnly},
    {TEXT("SnapshotInfo"), CrDeviceProperty_SnapshotInfo},
    {TEXT("BatteryRemain"), CrDeviceProperty_BatteryRemain},
    {TEXT("BatteryLevel"), CrDeviceProperty_BatteryLevel},
    {TEXT("EstimatePictureSize"), CrDeviceProperty_EstimatePictureSize},
    {TEXT("RecordingState"), CrDeviceProperty_RecordingState},
    {TEXT("LiveViewStatus"), CrDeviceProperty_LiveViewStatus},
    {TEXT("FocusIndication"), CrDeviceProperty_FocusIndication},
    {TEXT("MediaSLOT1_Status"), CrDeviceProperty_MediaSLOT1_Status},
    {TEXT("MediaSLOT1_RemainingNumber"), CrDeviceProperty_MediaSLOT1_RemainingNumber},
    {TEXT("MediaSLOT1_RemainingTime"), CrDeviceProperty_MediaSLOT1_RemainingTime},
    {TEXT("MediaSLOT1_FormatEnableStatus"), CrDeviceProperty_MediaSLOT1_FormatEnableStatus},
    {TEXT("MediaSLOT2_Status"), CrDeviceProperty_MediaSLOT2_Status},
    {TEXT("MediaSLOT2_FormatEnableStatus"), CrDeviceProperty_MediaSLOT2_FormatEnableStatus},
    {TEXT("MediaSLOT2_RemainingNumber"), CrDeviceProperty_MediaSLOT2_RemainingNumber},
    {TEXT("MediaSLOT2_RemainingTime"), CrDeviceProperty_MediaSLOT2_RemainingTime},
    {TEXT("Media_FormatProgressRate"), CrDeviceProperty_Media_FormatProgressRate},
    {TEXT("LiveView_Area"), CrDeviceProperty_LiveView_Area},
    {TEXT("Interval_Rec_Status"), CrDeviceProperty_Interval_Rec_Status},
    {TEXT("CustomWB_Execution_State"), CrDeviceProperty_CustomWB_Execution_State},
    {TEXT("CustomWB_Capturable_Area"), CrDeviceProperty_CustomWB_Capturable_Area},
    {TEXT("CustomWB_Capture_Frame_Size"), CrDeviceProperty_CustomWB_Capture_Frame_Size},
    {TEXT("CustomWB_Capture_Operation"), CrDeviceProperty_CustomWB_Capture_Operation},
    {TEXT("Zoom_Operation_Status"), CrDeviceProperty_Zoom_Operation_Status},
    {TEXT("Zoom_Bar_Information"), CrDeviceProperty_Zoom_Bar_Information},
    {TEXT("Zoom_Type_Status"), CrDeviceProperty_Zoom_Type_Status},
    {TEXT("MediaSLOT1_QuickFormatEnableStatus"), CrDeviceProperty_MediaSLOT1_QuickFormatEnableStatus},
    {TEXT("MediaSLOT2_QuickFormatEnableStatus"), CrDeviceProperty_MediaSLOT2_QuickFormatEnableStatus},
    {TEXT("Cancel_Media_FormatEnableStatus"), CrDeviceProperty_Cancel_Media_FormatEnableStatus},
    {TEXT("Zoom_Speed_Range"), CrDeviceProperty_Zoom_Speed_Range},
    {TEXT("SdkControlMode"), CrDeviceProperty_SdkControlMode},
    {TEXT("ContentsTransferStatus"), CrDeviceProperty_ContentsTransferStatus},
    {TEXT("ContentsTransferCancelEnableStatus"), CrDeviceProperty_ContentsTransferCancelEnableStatus},
    {TEXT("ContentsTransferProgress"), CrDeviceProperty_ContentsTransferProgress},
    {TEXT("MaxVal"), CrDeviceProperty_MaxVal}
};

const std::unordered_map<CrInt32u, text> map_property_names{
    {CrDeviceProperty_Undefined, TEXT("Undefined")},
    {CrDeviceProperty_S1, TEXT("S1")},
    {CrDeviceProperty_AEL, TEXT("AEL")},
    {CrDeviceProperty_FEL, TEXT("FEL")},
    {CrDeviceProperty_AFL, TEXT("AFL")},
    {CrDeviceProperty_AWBL, TEXT("AWBL")},
    {CrDeviceProperty_FNumber, TEXT("FNumber")},
    {CrDeviceProperty_ExposureBiasCompensation, TEXT("ExposureBiasCompensation")},
    {CrDeviceProperty_FlashCompensation, TEXT("FlashCompensation")},
    {CrDeviceProperty_ShutterSpeed, TEXT("ShutterSpeed")},
    {CrDeviceProperty_IsoSensitivity, TEXT("IsoSensitivity")},
    {CrDeviceProperty_ExposureProgramMode, TEXT("ExposureProgramMode")},
    {CrDeviceProperty_FileType, TEXT("FileType")},
    {CrDeviceProperty_JpegQuality, TEXT("JpegQuality")},
    {CrDeviceProperty_WhiteBalance, TEXT("WhiteBalance")},
    {CrDeviceProperty_FocusMode, TEXT("FocusMode")},
    {CrDeviceProperty_MeteringMode, TEXT("MeteringMode")},
    {CrDeviceProperty_FlashMode, TEXT("FlashMode")},
    {CrDeviceProperty_WirelessFlash, TEXT("WirelessFlash")},
    {CrDeviceProperty_RedEyeReduction, TEXT("RedEyeReduction")},
    {CrDeviceProperty_DriveMode, TEXT("DriveMode")},
    {CrDeviceProperty_DRO, TEXT("DRO")},
    {CrDeviceProperty_ImageSize, TEXT("ImageSize")},
    {CrDeviceProperty_AspectRatio, TEXT("AspectRatio")},
    {CrDeviceProperty_PictureEffect, TEXT("PictureEffect")},
    {CrDeviceProperty_FocusArea, TEXT("FocusArea")},
    {CrDeviceProperty_Colortemp, TEXT("Colortemp")},
    {CrDeviceProperty_ColorTuningAB, TEXT("ColorTuningAB")},
    {CrDeviceProperty_ColorTuningGM, TEXT("ColorTuningGM")},
    {CrDeviceProperty_LiveViewDisplayEffect, TEXT("LiveViewDisplayEffect")},
    {CrDeviceProperty_StillImageStoreDestination, TEXT("StillImageStoreDestination")},
    {CrDeviceProperty_PriorityKeySettings, TEXT("PriorityKeySettings")},
    {CrDeviceProperty_Focus_Magnifier_Setting, TEXT("Focus_Magnifier_Setting")},
    {CrDeviceProperty_DateTime_Settings, TEXT("DateTime_Settings")},
    {CrDeviceProperty_NearFar, TEXT("NearFar")},
    {CrDeviceProperty_AF_Area_Position, TEXT("AF_Area_Position")},
    {CrDeviceProperty_Zoom_Scale, TEXT("Zoom_Scale")},
    {CrDeviceProperty_Zoom_Setting, TEXT("Zoom_Setting")},
    {CrDeviceProperty_Zoom_Operation, TEXT("Zoom_Operation")},
    {CrDeviceProperty_Movie_File_Format, TEXT("Movie_File_Format")},
    {CrDeviceProperty_Movie_Recording_Setting, TEXT("Movie_Recording_Setting")},
    {CrDeviceProperty_Movie_Recording_FrameRateSetting, TEXT("Movie_Recording_FrameRateSetting")},
    {CrDeviceProperty_CompressionFileFormatStill, TEXT("CompressionFileFormatStill")},
    {CrDeviceProperty_MediaSLOT1_FileType, TEXT("MediaSLOT1_FileType")},
    {CrDeviceProperty_MediaSLOT2_FileType, TEXT("MediaSLOT2_FileType")},
    {CrDeviceProperty_MediaSLOT1_JpegQuality, TEXT("MediaSLOT1_JpegQuality")},
    {CrDeviceProperty_MediaSLOT2_JpegQuality, TEXT("MediaSLOT2_JpegQuality")},
    {CrDeviceProperty_MediaSLOT1_ImageSize, TEXT("MediaSLOT1_ImageSize")},
    {CrDeviceProperty_MediaSLOT2_ImageSize, TEXT("MediaSLOT2_ImageSize")},
    {CrDeviceProperty_RAW_FileCompressionType, TEXT("RAW_FileCompressionType")},
    {CrDeviceProperty_MediaSLOT1_RAW_FileCompressionType, TEXT("MediaSLOT1_RAW_FileCompressionType")},
    {CrDeviceProperty_MediaSLOT2_RAW_FileCompressionType, TEXT("MediaSLOT2_RAW_FileCompressionType")},
    {CrDeviceProperty_ZoomAndFocusPosition_Save, TEXT("ZoomAndFocusPosition_Save")},
    {CrDeviceProperty_ZoomAndFocusPosition_Load, TEXT("ZoomAndFocusPosition_Load")},
    {CrDeviceProperty_S2, TEXT("S2")},
    {CrDeviceProperty_Interval_Rec_Mode, TEXT("Interval_Rec_Mode")},
    {CrDeviceProperty_Still_Image_Trans_Size, TEXT("Still_Image_Trans_Size")},
    {CrDeviceProperty_RAW_J_PC_Save_Image, TEXT("RAW_J_PC_Save_Image")},
    {CrDeviceProperty_LiveView_Image_Quality, TEXT("LiveView_Image_Quality")},
    {CrDeviceProperty_CustomWB_Capture_Standby, TEXT("CustomWB_Capture_Standby")},
    {CrDeviceProperty_CustomWB_Capture_Standby_Cancel, TEXT("CustomWB_Capture_Standby_Cancel")},
    {CrDeviceProperty_CustomWB_Capture, TEXT("CustomWB_Capture")},
    {CrDeviceProperty_Remocon_Zoom_Speed_Type, TEXT("Remocon_Zoom_Speed_Type")},
    {CrDeviceProperty_GetOnly, TEXT("GetOnly")},
    {CrDeviceProperty_SnapshotInfo, TEXT("SnapshotInfo")},
    {CrDeviceProperty_BatteryRemain, TEXT("BatteryRemain")},
    {CrDeviceProperty_BatteryLevel, TEXT("BatteryLevel")},
    {CrDeviceProperty_EstimatePictureSize, TEXT("EstimatePictureSize")},
    {CrDeviceProperty_RecordingState, TEXT("RecordingState")},
    {CrDeviceProperty_LiveViewStatus, TEXT("LiveViewStatus")},
    {CrDeviceProperty_FocusIndication, TEXT("FocusIndication")},
    {CrDeviceProperty_MediaSLOT1_Status, TEXT("MediaSLOT1_Status")},
    {CrDeviceProperty_MediaSLOT1_RemainingNumber, TEXT("MediaSLOT1_RemainingNumber")},
    {CrDeviceProperty_MediaSLOT1_RemainingTime, TEXT("MediaSLOT1_RemainingTime")},
    {CrDeviceProperty_MediaSLOT1_FormatEnableStatus, TEXT("MediaSLOT1_FormatEnableStatus")},
    {CrDeviceProperty_MediaSLOT2_Status, TEXT("MediaSLOT2_Status")},
    {CrDeviceProperty_MediaSLOT2_FormatEnableStatus, TEXT("MediaSLOT2_FormatEnableStatus")},
    {CrDeviceProperty_MediaSLOT2_RemainingNumber, TEXT("MediaSLOT2_RemainingNumber")},
    {CrDeviceProperty_MediaSLOT2_RemainingTime, TEXT("MediaSLOT2_RemainingTime")},
    {CrDeviceProperty_Media_FormatProgressRate, TEXT("Media_FormatProgressRate")},
    {CrDeviceProperty_LiveView_Area, TEXT("LiveView_Area")},
    {CrDeviceProperty_Interval_Rec_Status, TEXT("Interval_Rec_Status")},
    {CrDeviceProperty_CustomWB_Execution_State, TEXT("CustomWB_Execution_State")},
    {CrDeviceProperty_CustomWB_Capturable_Area, TEXT("CustomWB_Capturable_Area")},
    {CrDeviceProperty_CustomWB_Capture_Frame_Size, TEXT("CustomWB_Capture_Frame_Size")},
    {CrDeviceProperty_CustomWB_Capture_Operation, TEXT("CustomWB_Capture_Operation")},
    {CrDeviceProperty_Zoom_Operation_Status, TEXT("Zoom_Operation_Status")},
    {CrDeviceProperty_Zoom_Bar_Information, TEXT("Zoom_Bar_Information")},
    {CrDeviceProperty_Zoom_Type_Status, TEXT("Zoom_Type_Status")},
    {CrDeviceProperty_MediaSLOT1_QuickFormatEnableStatus, TEXT("MediaSLOT1_QuickFormatEnableStatus")},
    {CrDeviceProperty_MediaSLOT2_QuickFormatEnableStatus, TEXT("MediaSLOT2_QuickFormatEnableStatus")},
    {CrDeviceProperty_Cancel_Media_FormatEnableStatus, TEXT("Cancel_Media_FormatEnableStatus")},
    {CrDeviceProperty_Zoom_Speed_Range, TEXT("Zoom_Speed_Range")},
    {CrDeviceProperty_SdkControlMode, TEXT("SdkControlMode")},
    {CrDeviceProperty_ContentsTransferStatus, TEXT("ContentsTransferStatus")},
    {CrDeviceProperty_ContentsTransferCancelEnableStatus, TEXT("ContentsTransferCancelEnableStatus")},
    {CrDeviceProperty_ContentsTransferProgress, TEXT("ContentsTransferProgress")},
    {CrDeviceProperty_MaxVal, TEXT("MaxVal")}
};

void releaseExitSuccess() {
    SDK::Release();
    std::exit(EXIT_SUCCESS);
}

void releaseExitFailure() {
    SDK::Release();
    std::exit(EXIT_FAILURE);
}

CameraDevicePtr getCamera(bool verbose) 
{
    // Change global locale to native locale
    std::locale::global(std::locale(""));

    // Make the stream's locale the same as the current global locale
    tin.imbue(std::locale());
    tout.imbue(std::locale());

    auto init_success = SDK::Init();
    if (!init_success) {
        tout << "Error: Failed to initialize Remote SDK\n";
        releaseExitFailure();
    }
     if (verbose) tout << "Remote SDK successfully initialized.\n";

    SDK::ICrEnumCameraObjectInfo* camera_list = nullptr;

    auto enum_status = SDK::EnumCameraObjects(&camera_list, 0);
    if (CR_FAILED(enum_status) || camera_list == nullptr) {
        tout << "Error: No cameras detected\n";
        releaseExitFailure();
    }
    auto ncams = camera_list->GetCount();

     if (verbose) tout << "Cameras detectd: " << ncams << "\n";

    std::int32_t cameraNumUniq = 1;
    std::int32_t selectCamera = 1;
    std::int8_t no = 1;

    auto* camera_info = camera_list->GetCameraObjectInfo(no - 1);

    CameraDevicePtr camera = CameraDevicePtr(new CameraDevice(cameraNumUniq, nullptr, camera_info));
    camera->releaseExitSuccess = releaseExitSuccess;
    
    camera->set_verbose(verbose);

    camera_list->Release();

    if (!camera->connect(SDK::CrSdkControlMode_Remote)) {
        tout << "Error: Unable to connect to camera\n";
        return nullptr;
    }
    std::this_thread::sleep_for(1000ms);
    if (verbose) tout << "Camera connected\n";
    return camera;
}

void capture(string dir, bool verbose)
{
    CameraDevicePtr camera = getCamera(verbose);
    if (camera == nullptr) releaseExitFailure();

    camera->set_release_after_download(true);

    text textDir(dir.begin(), dir.end());

    if (dir.length() > 0) {
        camera->set_save_path(textDir, TEXT(""), -1);
    }
    camera->half_full_release();
    std::this_thread::sleep_for(5000ms);
    tout << "Error: Unable to download image\n";
    releaseExitFailure();
}

void getProperty(const string &prop, bool verbose) {
    text propText(prop.begin(), prop.end());

    if (map_device_property.count(propText) == 0) {
        tout << "Error: Property not found\n";
        releaseExitFailure();
    }

    CameraDevicePtr camera = getCamera(verbose);

    if (camera == nullptr) releaseExitFailure();
    
    CrInt32u code = map_device_property.at(propText);
    CrInt64 value = 0;
    if (!camera->get_property_value(code, value)) releaseExitFailure();
    tout << propText << ": " << value << "\n";
    releaseExitSuccess();
}

void setProperty(const string &prop, const string &value, bool verbose)
{
    text propText(prop.begin(), prop.end());

    if (map_device_property.count(propText) == 0) {
        tout << "Error: Property not found\n";
        releaseExitFailure();
    }

    CameraDevicePtr camera = getCamera(verbose);
    if (camera == nullptr) releaseExitFailure();

    CrInt32u code = map_device_property.at(propText);

    if (!camera->set_property_value(code, std::stoi(value))) {
        tout << "Error: Unable to set property\n";
        releaseExitFailure();
    }
    releaseExitSuccess();
}

mode ArgParser(int argc, char* argv[])
{
    mode selected = mode::help;
    bool verbose = false;

    string dir;
    string prop;
    string val;

    auto captureCommand = (
        command("capture").set(selected, mode::capture).doc("Capture an image"),
        option("--dir").doc("Output dir") & value("output dir", dir)
    );

    auto getCommand = (
        command("get").set(selected, mode::get).doc("Gets the value of a camera property"),
        required("--prop") & value("prop", prop)
    );

    auto setCommand = (
        command("set").set(selected, mode::set).doc("Sets the value of camera property"),
        required("--prop").doc("Property name") & value("prop", prop),
        required("--value").doc("Property value") & value("value", val)
    );

    auto cli = (
        captureCommand |
        getCommand |
        setCommand |
        command("sdk").set(selected, mode::sdk).doc("Load the sample app from Sony Camera SDK") |
        command("--help").set(selected, mode::help).doc("This printed message"),
        option("--verbose").set(verbose, true).doc("Prints debugging messages")
    );

    if(parse(argc, argv, cli)) {
        switch(selected) {
            case mode::capture:
                capture(dir, verbose);
                break;
            case mode::get:
                getProperty(prop, verbose);
                break;
            case mode::set:
                setProperty(prop, val, verbose);
                break;
            case mode::sdk:
                return mode::sdk;
                break;
            case mode::help:
                std::cout << make_man_page(cli, argv[0]);
                break;
        }
    } else {
        tout << "Try --help to see usage\n";
    }
    std::exit(EXIT_FAILURE);
}