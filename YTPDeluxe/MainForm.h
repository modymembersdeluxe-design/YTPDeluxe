#pragma once

#include "ProjectModel.h"
#include "FfmpegPipeline.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Windows::Forms;

namespace YTPDeluxe
{
    public ref class MainForm : public Form
    {
    public:
        MainForm();

    private:
        ProjectSettings^ settings;
        TabControl^ tabControl;
        ListView^ sourceList;
        ListView^ urlList;
        CheckedListBox^ audioEffectsList;
        CheckedListBox^ videoEffectsList;
        ComboBox^ projectTypeCombo;
        ComboBox^ styleCombo;
        NumericUpDown^ durationInput;
        NumericUpDown^ minStreamInput;
        NumericUpDown^ maxStreamInput;
        NumericUpDown^ clipCountInput;
        TextBox^ outputPathText;
        TextBox^ statusText;
        TextBox^ planText;
        TextBox^ logText;
        ListView^ assetFolderList;
        Button^ previewButton;
        Button^ exportButton;

        void InitializeComponent();
        void InitializeEffectLists();
        void UpdateStatus();
        void Log(String^ message);
        void AddSource(SourceType type, String^ path);
        void RefreshAssetFolders();
        EffectFlags GetSelectedEffects();

        void OnAddVideo(Object^ sender, EventArgs^ e);
        void OnAddAudio(Object^ sender, EventArgs^ e);
        void OnAddImage(Object^ sender, EventArgs^ e);
        void OnAddUrl(Object^ sender, EventArgs^ e);
        void OnBrowseAssets(Object^ sender, EventArgs^ e);
        void OnPreview(Object^ sender, EventArgs^ e);
        void OnExport(Object^ sender, EventArgs^ e);
        void OnBrowseOutput(Object^ sender, EventArgs^ e);
        void OnGeneratePlan(Object^ sender, EventArgs^ e);
        void OnSettingsChanged(Object^ sender, EventArgs^ e);
    };
}
