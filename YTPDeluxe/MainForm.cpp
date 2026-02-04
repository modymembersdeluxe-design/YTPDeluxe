#include "MainForm.h"
#using <Microsoft.VisualBasic.dll>

namespace YTPDeluxe
{
    MainForm::MainForm()
    {
        settings = gcnew ProjectSettings();
        InitializeComponent();
        InitializeEffectLists();
        UpdateStatus();
    }

    void MainForm::InitializeComponent()
    {
        Text = "YTP Deluxe - Advanced YouTube Poop Studio";
        Size = Drawing::Size(1200, 820);
        MinimumSize = Drawing::Size(1100, 720);

        tabControl = gcnew TabControl();
        tabControl->Dock = DockStyle::Fill;

        TabPage^ sourcesTab = gcnew TabPage("Sources");
        TabPage^ effectsTab = gcnew TabPage("Effects");
        TabPage^ generatorTab = gcnew TabPage("Generator");
        TabPage^ exportTab = gcnew TabPage("Export");
        TabPage^ logsTab = gcnew TabPage("Logs");

        tabControl->TabPages->AddRange(gcnew array<TabPage^>{ sourcesTab, effectsTab, generatorTab, exportTab, logsTab });
        Controls->Add(tabControl);

        Button^ addVideoButton = gcnew Button();
        addVideoButton->Text = "Add Video";
        addVideoButton->Location = Point(16, 18);
        addVideoButton->Click += gcnew EventHandler(this, &MainForm::OnAddVideo);

        Button^ addAudioButton = gcnew Button();
        addAudioButton->Text = "Add Audio";
        addAudioButton->Location = Point(120, 18);
        addAudioButton->Click += gcnew EventHandler(this, &MainForm::OnAddAudio);

        Button^ addImageButton = gcnew Button();
        addImageButton->Text = "Add Image/GIF";
        addImageButton->Location = Point(224, 18);
        addImageButton->Click += gcnew EventHandler(this, &MainForm::OnAddImage);

        Button^ addUrlButton = gcnew Button();
        addUrlButton->Text = "Register URL";
        addUrlButton->Location = Point(352, 18);
        addUrlButton->Click += gcnew EventHandler(this, &MainForm::OnAddUrl);

        Button^ browseAssetsButton = gcnew Button();
        browseAssetsButton->Text = "Asset Folders";
        browseAssetsButton->Location = Point(472, 18);
        browseAssetsButton->Click += gcnew EventHandler(this, &MainForm::OnBrowseAssets);

        sourceList = gcnew ListView();
        sourceList->Location = Point(16, 60);
        sourceList->Size = Drawing::Size(740, 560);
        sourceList->View = View::Details;
        sourceList->Columns->Add("Name", 200);
        sourceList->Columns->Add("Path", 360);
        sourceList->Columns->Add("Type", 120);
        sourceList->FullRowSelect = true;

        urlList = gcnew ListView();
        urlList->Location = Point(776, 60);
        urlList->Size = Drawing::Size(360, 220);
        urlList->View = View::Details;
        urlList->Columns->Add("URL", 320);
        urlList->FullRowSelect = true;

        assetFolderList = gcnew ListView();
        assetFolderList->Location = Point(776, 310);
        assetFolderList->Size = Drawing::Size(360, 310);
        assetFolderList->View = View::Details;
        assetFolderList->Columns->Add("Asset", 120);
        assetFolderList->Columns->Add("Folder", 220);
        assetFolderList->FullRowSelect = true;

        sourcesTab->Controls->Add(addVideoButton);
        sourcesTab->Controls->Add(addAudioButton);
        sourcesTab->Controls->Add(addImageButton);
        sourcesTab->Controls->Add(addUrlButton);
        sourcesTab->Controls->Add(browseAssetsButton);
        sourcesTab->Controls->Add(sourceList);
        sourcesTab->Controls->Add(urlList);
        sourcesTab->Controls->Add(assetFolderList);

        GroupBox^ audioGroup = gcnew GroupBox();
        audioGroup->Text = "Audio Effects";
        audioGroup->Location = Point(16, 16);
        audioGroup->Size = Drawing::Size(520, 600);

        audioEffectsList = gcnew CheckedListBox();
        audioEffectsList->Dock = DockStyle::Fill;
        audioEffectsList->CheckOnClick = true;
        audioEffectsList->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);
        audioGroup->Controls->Add(audioEffectsList);

        GroupBox^ videoGroup = gcnew GroupBox();
        videoGroup->Text = "Video Effects";
        videoGroup->Location = Point(552, 16);
        videoGroup->Size = Drawing::Size(520, 600);

        videoEffectsList = gcnew CheckedListBox();
        videoEffectsList->Dock = DockStyle::Fill;
        videoEffectsList->CheckOnClick = true;
        videoEffectsList->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);
        videoGroup->Controls->Add(videoEffectsList);

        effectsTab->Controls->Add(audioGroup);
        effectsTab->Controls->Add(videoGroup);

        Label^ projectTypeLabel = gcnew Label();
        projectTypeLabel->Text = "Project Type";
        projectTypeLabel->Location = Point(16, 20);

        projectTypeCombo = gcnew ComboBox();
        projectTypeCombo->Location = Point(140, 16);
        projectTypeCombo->DropDownStyle = ComboBoxStyle::DropDownList;
        projectTypeCombo->Items->AddRange(gcnew array<Object^>{ "Generic", "YTP Tennis", "Collab Entry", "YTPMV" });
        projectTypeCombo->SelectedIndex = 0;
        projectTypeCombo->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Label^ styleLabel = gcnew Label();
        styleLabel->Text = "YTP Style";
        styleLabel->Location = Point(16, 60);

        styleCombo = gcnew ComboBox();
        styleCombo->Location = Point(140, 56);
        styleCombo->DropDownStyle = ComboBoxStyle::DropDownList;
        styleCombo->Items->AddRange(gcnew array<Object^>{ "Classic (2007-2012)", "Modern (2013-2021)", "Advanced Experimental" });
        styleCombo->SelectedIndex = 0;
        styleCombo->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Label^ durationLabel = gcnew Label();
        durationLabel->Text = "Target Duration (sec)";
        durationLabel->Location = Point(16, 100);

        durationInput = gcnew NumericUpDown();
        durationInput->Location = Point(200, 96);
        durationInput->Minimum = 10;
        durationInput->Maximum = 36000;
        durationInput->Value = 120;
        durationInput->ValueChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Label^ minStreamLabel = gcnew Label();
        minStreamLabel->Text = "Min Stream (sec)";
        minStreamLabel->Location = Point(16, 140);

        minStreamInput = gcnew NumericUpDown();
        minStreamInput->Location = Point(200, 136);
        minStreamInput->Minimum = 0.1M;
        minStreamInput->Maximum = 30;
        minStreamInput->DecimalPlaces = 1;
        minStreamInput->Increment = 0.5M;
        minStreamInput->Value = 0.5M;
        minStreamInput->ValueChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Label^ maxStreamLabel = gcnew Label();
        maxStreamLabel->Text = "Max Stream (sec)";
        maxStreamLabel->Location = Point(16, 180);

        maxStreamInput = gcnew NumericUpDown();
        maxStreamInput->Location = Point(200, 176);
        maxStreamInput->Minimum = 0.5M;
        maxStreamInput->Maximum = 60;
        maxStreamInput->DecimalPlaces = 1;
        maxStreamInput->Increment = 0.5M;
        maxStreamInput->Value = 6.0M;
        maxStreamInput->ValueChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Label^ clipCountLabel = gcnew Label();
        clipCountLabel->Text = "Clip Count";
        clipCountLabel->Location = Point(16, 220);

        clipCountInput = gcnew NumericUpDown();
        clipCountInput->Location = Point(200, 216);
        clipCountInput->Minimum = 1;
        clipCountInput->Maximum = 70000;
        clipCountInput->Value = 300;
        clipCountInput->ValueChanged += gcnew EventHandler(this, &MainForm::OnSettingsChanged);

        Button^ generatePlanButton = gcnew Button();
        generatePlanButton->Text = "Generate Randomized Plan";
        generatePlanButton->Location = Point(16, 260);
        generatePlanButton->Size = Drawing::Size(260, 34);
        generatePlanButton->Click += gcnew EventHandler(this, &MainForm::OnGeneratePlan);

        planText = gcnew TextBox();
        planText->Location = Point(16, 310);
        planText->Size = Drawing::Size(520, 300);
        planText->Multiline = true;
        planText->ScrollBars = ScrollBars::Vertical;
        planText->ReadOnly = true;

        generatorTab->Controls->Add(projectTypeLabel);
        generatorTab->Controls->Add(projectTypeCombo);
        generatorTab->Controls->Add(styleLabel);
        generatorTab->Controls->Add(styleCombo);
        generatorTab->Controls->Add(durationLabel);
        generatorTab->Controls->Add(durationInput);
        generatorTab->Controls->Add(minStreamLabel);
        generatorTab->Controls->Add(minStreamInput);
        generatorTab->Controls->Add(maxStreamLabel);
        generatorTab->Controls->Add(maxStreamInput);
        generatorTab->Controls->Add(clipCountLabel);
        generatorTab->Controls->Add(clipCountInput);
        generatorTab->Controls->Add(generatePlanButton);
        generatorTab->Controls->Add(planText);

        GroupBox^ exportGroup = gcnew GroupBox();
        exportGroup->Text = "Render";
        exportGroup->Location = Point(16, 16);
        exportGroup->Size = Drawing::Size(1120, 160);

        Label^ outputLabel = gcnew Label();
        outputLabel->Text = "Output";
        outputLabel->Location = Point(20, 32);

        outputPathText = gcnew TextBox();
        outputPathText->Location = Point(80, 28);
        outputPathText->Size = Drawing::Size(820, 26);
        outputPathText->Text = "C:\\YTPDeluxe\\render.mp4";

        Button^ browseOutputButton = gcnew Button();
        browseOutputButton->Text = "Browse";
        browseOutputButton->Location = Point(920, 26);
        browseOutputButton->Click += gcnew EventHandler(this, &MainForm::OnBrowseOutput);

        previewButton = gcnew Button();
        previewButton->Text = "Preview";
        previewButton->Location = Point(80, 70);
        previewButton->Size = Drawing::Size(120, 34);
        previewButton->Click += gcnew EventHandler(this, &MainForm::OnPreview);

        exportButton = gcnew Button();
        exportButton->Text = "Export via FFmpeg";
        exportButton->Location = Point(220, 70);
        exportButton->Size = Drawing::Size(180, 34);
        exportButton->Click += gcnew EventHandler(this, &MainForm::OnExport);

        statusText = gcnew TextBox();
        statusText->Location = Point(16, 190);
        statusText->Size = Drawing::Size(1120, 420);
        statusText->Multiline = true;
        statusText->ReadOnly = true;
        statusText->ScrollBars = ScrollBars::Vertical;

        exportGroup->Controls->Add(outputLabel);
        exportGroup->Controls->Add(outputPathText);
        exportGroup->Controls->Add(browseOutputButton);
        exportGroup->Controls->Add(previewButton);
        exportGroup->Controls->Add(exportButton);

        exportTab->Controls->Add(exportGroup);
        exportTab->Controls->Add(statusText);

        logText = gcnew TextBox();
        logText->Dock = DockStyle::Fill;
        logText->Multiline = true;
        logText->ReadOnly = true;
        logText->ScrollBars = ScrollBars::Vertical;
        logsTab->Controls->Add(logText);

        RefreshAssetFolders();
    }

    void MainForm::InitializeEffectLists()
    {
        array<String^>^ audioEffects = gcnew array<String^>
        {
            "Random Sound", "Mute", "Speed Up", "Slow Down", "Reverse", "Chorus", "Vibrato",
            "Stutter", "Dance", "Squidward", "Sus", "Lagfun", "Low Harmony", "High Harmony",
            "Confusion", "Random Chords", "Trailing Reverses", "Low Quality Meme", "Audio Crust",
            "Pitch-Shift Loop", "Mashup Mixing"
        };

        array<String^>^ videoEffects = gcnew array<String^>
        {
            "Invert", "Rainbow", "Mirror", "Mirror Symmetry", "Screen Clip", "Overlay Images",
            "Spadinner", "Sentence Mixing", "Shuffle Frames", "Framerate Reduction", "Random Cuts"
        };

        audioEffectsList->Items->AddRange(audioEffects);
        videoEffectsList->Items->AddRange(videoEffects);
    }

    void MainForm::UpdateStatus()
    {
        settings->TargetDurationSeconds = Decimal::ToDouble(durationInput->Value);
        settings->MinStreamSeconds = Decimal::ToDouble(minStreamInput->Value);
        settings->MaxStreamSeconds = Decimal::ToDouble(maxStreamInput->Value);
        settings->ClipCount = Decimal::ToInt32(clipCountInput->Value);
        settings->Effects = GetSelectedEffects();

        String^ status = String::Format(
            "Project: {0}\r\nType: {1}\r\nStyle: {2}\r\nDuration: {3} sec\r\nStream Range: {4}-{5} sec\r\nClip Count: {6}\r\nSources: {7}\r\nOnline URLs: {8}\r\nMixer Channels: {9}\r\nEffects: {10}",
            settings->ProjectName,
            projectTypeCombo->SelectedItem,
            styleCombo->SelectedItem,
            settings->TargetDurationSeconds,
            settings->MinStreamSeconds,
            settings->MaxStreamSeconds,
            settings->ClipCount,
            settings->Sources->Count,
            settings->OnlineUrls->Count,
            settings->MixerChannels->Count,
            FfmpegPipeline::BuildEffectSummary(settings->Effects));

        statusText->Text = status;
    }

    void MainForm::Log(String^ message)
    {
        logText->AppendText(String::Format("[{0}] {1}\r\n", DateTime::Now.ToString("HH:mm:ss"), message));
    }

    void MainForm::AddSource(SourceType type, String^ path)
    {
        if (String::IsNullOrWhiteSpace(path))
        {
            return;
        }

        String^ name = Path::GetFileName(path);
        SourceItem^ item = gcnew SourceItem(name, path, type);
        settings->Sources->Add(item);

        ListViewItem^ row = gcnew ListViewItem(name);
        row->SubItems->Add(path);
        row->SubItems->Add(type.ToString());
        sourceList->Items->Add(row);
        UpdateStatus();
        Log(String::Format("Added {0}: {1}", type, path));
    }

    void MainForm::RefreshAssetFolders()
    {
        array<String^>^ defaultAssets = gcnew array<String^>
        {
            "images", "memes", "meme_sounds", "sounds", "overlay_videos", "adverts", "errors"
        };

        for each (String ^ key in defaultAssets)
        {
            if (!settings->AssetFolders->ContainsKey(key))
            {
                settings->AssetFolders->Add(key, "");
            }
        }

        assetFolderList->Items->Clear();
        for each (KeyValuePair<String^, String^> entry in settings->AssetFolders)
        {
            ListViewItem^ row = gcnew ListViewItem(entry.Key);
            row->SubItems->Add(entry.Value);
            assetFolderList->Items->Add(row);
        }
    }

    EffectFlags MainForm::GetSelectedEffects()
    {
        EffectFlags flags = EffectFlags::None;

        for each (String ^ effect in audioEffectsList->CheckedItems)
        {
            if (effect == "Random Sound") flags = flags | EffectFlags::AudioRandomSound;
            else if (effect == "Mute") flags = flags | EffectFlags::AudioMute;
            else if (effect == "Speed Up") flags = flags | EffectFlags::AudioSpeedUp;
            else if (effect == "Slow Down") flags = flags | EffectFlags::AudioSlowDown;
            else if (effect == "Reverse") flags = flags | EffectFlags::AudioReverse;
            else if (effect == "Chorus") flags = flags | EffectFlags::AudioChorus;
            else if (effect == "Vibrato") flags = flags | EffectFlags::AudioVibrato;
            else if (effect == "Stutter") flags = flags | EffectFlags::AudioStutter;
            else if (effect == "Dance") flags = flags | EffectFlags::AudioDance;
            else if (effect == "Squidward") flags = flags | EffectFlags::AudioSquidward;
            else if (effect == "Sus") flags = flags | EffectFlags::AudioSus;
            else if (effect == "Lagfun") flags = flags | EffectFlags::AudioLagfun;
            else if (effect == "Low Harmony") flags = flags | EffectFlags::AudioLowHarmony;
            else if (effect == "High Harmony") flags = flags | EffectFlags::AudioHighHarmony;
            else if (effect == "Confusion") flags = flags | EffectFlags::AudioConfusion;
            else if (effect == "Random Chords") flags = flags | EffectFlags::AudioRandomChords;
            else if (effect == "Trailing Reverses") flags = flags | EffectFlags::AudioTrailingReverses;
            else if (effect == "Low Quality Meme") flags = flags | EffectFlags::AudioLowQualityMeme;
            else if (effect == "Audio Crust") flags = flags | EffectFlags::AudioCrust;
            else if (effect == "Pitch-Shift Loop") flags = flags | EffectFlags::AudioPitchShiftLoop;
            else if (effect == "Mashup Mixing") flags = flags | EffectFlags::AudioMashupMixing;
        }

        for each (String ^ effect in videoEffectsList->CheckedItems)
        {
            if (effect == "Invert") flags = flags | EffectFlags::VideoInvert;
            else if (effect == "Rainbow") flags = flags | EffectFlags::VideoRainbow;
            else if (effect == "Mirror") flags = flags | EffectFlags::VideoMirror;
            else if (effect == "Mirror Symmetry") flags = flags | EffectFlags::VideoMirrorSymmetry;
            else if (effect == "Screen Clip") flags = flags | EffectFlags::VideoScreenClip;
            else if (effect == "Overlay Images") flags = flags | EffectFlags::VideoOverlayImages;
            else if (effect == "Spadinner") flags = flags | EffectFlags::VideoSpadinner;
            else if (effect == "Sentence Mixing") flags = flags | EffectFlags::VideoSentenceMixing;
            else if (effect == "Shuffle Frames") flags = flags | EffectFlags::VideoShuffleFrames;
            else if (effect == "Framerate Reduction") flags = flags | EffectFlags::VideoFrameRateReduction;
            else if (effect == "Random Cuts") flags = flags | EffectFlags::VideoRandomCuts;
        }

        return flags;
    }

    void MainForm::OnAddVideo(Object^, EventArgs^)
    {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Filter = "Video Files|*.mp4;*.wmv;*.avi;*.mkv|All Files|*.*";
        dialog->Multiselect = true;

        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            for each (String ^ file in dialog->FileNames)
            {
                AddSource(SourceType::Video, file);
            }
        }
    }

    void MainForm::OnAddAudio(Object^, EventArgs^)
    {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Filter = "Audio Files|*.mp3;*.wav;*.ogg|All Files|*.*";
        dialog->Multiselect = true;

        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            for each (String ^ file in dialog->FileNames)
            {
                AddSource(SourceType::Audio, file);
            }
        }
    }

    void MainForm::OnAddImage(Object^, EventArgs^)
    {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Filter = "Image Files|*.png;*.jpg;*.jpeg;*.webp;*.gif|All Files|*.*";
        dialog->Multiselect = true;

        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            for each (String ^ file in dialog->FileNames)
            {
                SourceType type = Path::GetExtension(file)->ToLower()->Equals(".gif") ? SourceType::Gif : SourceType::Image;
                AddSource(type, file);
            }
        }
    }

    void MainForm::OnAddUrl(Object^, EventArgs^)
    {
        String^ url = Microsoft::VisualBasic::Interaction::InputBox("Paste a media URL to register.", "Register URL", "https://");
        if (String::IsNullOrWhiteSpace(url))
        {
            return;
        }

        settings->OnlineUrls->Add(url);
        ListViewItem^ row = gcnew ListViewItem(url);
        urlList->Items->Add(row);
        UpdateStatus();
        Log(String::Format("Registered URL: {0}", url));
    }

    void MainForm::OnBrowseAssets(Object^, EventArgs^)
    {
        if (assetFolderList->SelectedItems->Count == 0)
        {
            MessageBox::Show("Select an asset row to assign a folder (images, memes, sounds, overlays).", "Asset Folders");
            return;
        }

        FolderBrowserDialog^ dialog = gcnew FolderBrowserDialog();
        if (dialog->ShowDialog() != System::Windows::Forms::DialogResult::OK)
        {
            return;
        }

        ListViewItem^ item = assetFolderList->SelectedItems[0];
        String^ key = item->SubItems[0]->Text;
        settings->AssetFolders[key] = dialog->SelectedPath;
        RefreshAssetFolders();
        Log(String::Format("Updated asset folder {0}: {1}", key, dialog->SelectedPath));
    }

    void MainForm::OnPreview(Object^, EventArgs^)
    {
        if (settings->Sources->Count == 0)
        {
            MessageBox::Show("Add at least one source clip to preview.", "Preview");
            return;
        }

        String^ path = settings->Sources[0]->Path;
        PipelineCommand^ preview = FfmpegPipeline::BuildPreviewCommand(path, GetSelectedEffects());
        Log(String::Format("Preview command: {0} {1}", preview->Executable, preview->Arguments));

        try
        {
            ProcessStartInfo^ info = gcnew ProcessStartInfo(preview->Executable, preview->Arguments);
            info->UseShellExecute = true;
            Process::Start(info);
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Unable to launch preview. Ensure FFplay or FFmpeg is installed.\n" + ex->Message, "Preview");
        }
    }

    void MainForm::OnExport(Object^, EventArgs^)
    {
        SaveFileDialog^ dialog = gcnew SaveFileDialog();
        dialog->Filter = "Video Files|*.mp4;*.wmv;*.avi;*.mkv|All Files|*.*";
        dialog->FileName = outputPathText->Text;

        if (dialog->ShowDialog() != System::Windows::Forms::DialogResult::OK)
        {
            return;
        }

        outputPathText->Text = dialog->FileName;

        if (String::IsNullOrWhiteSpace(outputPathText->Text))
        {
            return;
        }

        if (settings->Sources->Count == 0)
        {
            MessageBox::Show("Add sources before exporting.", "Export");
            return;
        }

        List<String^>^ clips = gcnew List<String^>();
        for each (SourceItem ^ item in settings->Sources)
        {
            clips->Add(item->Path);
        }

        String^ concatPath = Path::Combine(Path::GetTempPath(), "ytp_concat.txt");
        FfmpegPipeline::WriteConcatList(concatPath, clips);

        PipelineCommand^ exportCommand = FfmpegPipeline::BuildExportCommand(concatPath, outputPathText->Text, GetSelectedEffects());
        Log(String::Format("Export command: {0} {1}", exportCommand->Executable, exportCommand->Arguments));

        try
        {
            ProcessStartInfo^ info = gcnew ProcessStartInfo(exportCommand->Executable, exportCommand->Arguments);
            info->UseShellExecute = true;
            Process::Start(info);
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Unable to launch export. Ensure FFmpeg is installed.\n" + ex->Message, "Export");
        }
    }

    void MainForm::OnBrowseOutput(Object^, EventArgs^)
    {
        SaveFileDialog^ dialog = gcnew SaveFileDialog();
        dialog->Filter = "Video Files|*.mp4;*.wmv;*.avi;*.mkv|All Files|*.*";
        dialog->FileName = outputPathText->Text;

        if (dialog->ShowDialog() != System::Windows::Forms::DialogResult::OK)
        {
            return;
        }

        outputPathText->Text = dialog->FileName;
    }

    void MainForm::OnGeneratePlan(Object^, EventArgs^)
    {
        Random^ rng = gcnew Random();
        List<String^>^ plan = gcnew List<String^>();

        plan->Add("YTP Deluxe Plan");
        plan->Add(String::Format("Project Type: {0}", projectTypeCombo->SelectedItem));
        plan->Add(String::Format("Style: {0}", styleCombo->SelectedItem));
        plan->Add(String::Format("Target Duration: {0} sec", durationInput->Value));
        plan->Add("--- Segments ---");

        int segments = Math::Min(12, settings->Sources->Count + 3);
        for (int i = 0; i < segments; i++)
        {
            int clipIndex = settings->Sources->Count == 0 ? 0 : rng->Next(settings->Sources->Count);
            String^ clipName = settings->Sources->Count == 0 ? "(No source yet)" : settings->Sources[clipIndex]->Name;
            double length = Math::Round(settings->MinStreamSeconds + rng->NextDouble() * (settings->MaxStreamSeconds - settings->MinStreamSeconds), 2);
            plan->Add(String::Format("Segment {0}: {1} | {2} sec | Effects: {3}", i + 1, clipName, length, FfmpegPipeline::BuildEffectSummary(GetSelectedEffects())));
        }

        planText->Text = String::Join("\r\n", plan);
        Log("Generated randomized plan.");
    }

    void MainForm::OnSettingsChanged(Object^, EventArgs^)
    {
        UpdateStatus();
    }
}
