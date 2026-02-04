#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace YTPDeluxe
{
    public enum class ProjectType
    {
        Generic,
        YtpTennis,
        CollabEntry,
        YtpMv
    };

    public enum class SourceType
    {
        Video,
        Audio,
        Image,
        Gif,
        Tracker,
        OnlineUrl
    };

    [Flags]
    public enum class EffectFlags
    {
        None = 0,
        AudioRandomSound = 1 << 0,
        AudioMute = 1 << 1,
        AudioSpeedUp = 1 << 2,
        AudioSlowDown = 1 << 3,
        AudioReverse = 1 << 4,
        AudioChorus = 1 << 5,
        AudioVibrato = 1 << 6,
        AudioStutter = 1 << 7,
        AudioDance = 1 << 8,
        AudioSquidward = 1 << 9,
        AudioSus = 1 << 10,
        AudioLagfun = 1 << 11,
        AudioLowHarmony = 1 << 12,
        AudioHighHarmony = 1 << 13,
        AudioConfusion = 1 << 14,
        AudioRandomChords = 1 << 15,
        AudioTrailingReverses = 1 << 16,
        AudioLowQualityMeme = 1 << 17,
        AudioCrust = 1 << 18,
        AudioPitchShiftLoop = 1 << 19,
        AudioMashupMixing = 1 << 20,
        VideoInvert = 1 << 21,
        VideoRainbow = 1 << 22,
        VideoMirror = 1 << 23,
        VideoMirrorSymmetry = 1 << 24,
        VideoScreenClip = 1 << 25,
        VideoOverlayImages = 1 << 26,
        VideoSpadinner = 1 << 27,
        VideoSentenceMixing = 1 << 28,
        VideoShuffleFrames = 1 << 29,
        VideoFrameRateReduction = 1 << 30,
        VideoRandomCuts = 1 << 31
    };

    public ref class SourceItem
    {
    public:
        SourceItem(String^ name, String^ path, SourceType type)
            : Name(name), Path(path), Type(type)
        {
            Tags = gcnew List<String^>();
        }

        property String^ Name;
        property String^ Path;
        property SourceType Type;
        property List<String^>^ Tags;
    };

    public ref class MixerChannel
    {
    public:
        MixerChannel(String^ name)
            : Name(name), GainDb(0.0), Muted(false)
        {
        }

        property String^ Name;
        property double GainDb;
        property bool Muted;
    };

    public ref class ProjectSettings
    {
    public:
        ProjectSettings()
        {
            ProjectName = "Untitled YTP";
            Type = ProjectType::Generic;
            TargetDurationSeconds = 120;
            MinStreamSeconds = 0.5;
            MaxStreamSeconds = 6.0;
            ClipCount = 300;
            Effects = EffectFlags::None;
            Sources = gcnew List<SourceItem^>();
            OnlineUrls = gcnew List<String^>();
            MixerChannels = gcnew List<MixerChannel^>();
            MixerChannels->Add(gcnew MixerChannel("Master"));
            MixerChannels->Add(gcnew MixerChannel("Aux1"));
            MixerChannels->Add(gcnew MixerChannel("Aux2"));
            MixerChannels->Add(gcnew MixerChannel("MIDI1"));
            MixerChannels->Add(gcnew MixerChannel("MIDI2"));
            MixerChannels->Add(gcnew MixerChannel("MIDI3"));
            AssetFolders = gcnew Dictionary<String^, String^>();
        }

        property String^ ProjectName;
        property ProjectType Type;
        property double TargetDurationSeconds;
        property double MinStreamSeconds;
        property double MaxStreamSeconds;
        property int ClipCount;
        property EffectFlags Effects;
        property List<SourceItem^>^ Sources;
        property List<String^>^ OnlineUrls;
        property List<MixerChannel^>^ MixerChannels;
        property Dictionary<String^, String^>^ AssetFolders;
    };
}
