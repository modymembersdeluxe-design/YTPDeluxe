#include "FfmpegPipeline.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Text;

namespace YTPDeluxe
{
    String^ FfmpegPipeline::BuildFilterGraph(EffectFlags flags)
    {
        List<String^>^ filters = gcnew List<String^>();

        if (flags.HasFlag(EffectFlags::VideoInvert))
        {
            filters->Add("negate");
        }
        if (flags.HasFlag(EffectFlags::VideoRainbow))
        {
            filters->Add("hue=s=2");
        }
        if (flags.HasFlag(EffectFlags::VideoMirror))
        {
            filters->Add("hflip");
        }
        if (flags.HasFlag(EffectFlags::VideoMirrorSymmetry))
        {
            filters->Add("split [a][b]; [b] hflip [b2]; [a][b2] hstack");
        }
        if (flags.HasFlag(EffectFlags::VideoScreenClip))
        {
            filters->Add("crop=in_w:in_h-120:0:60");
        }
        if (flags.HasFlag(EffectFlags::VideoFrameRateReduction))
        {
            filters->Add("fps=15");
        }
        if (flags.HasFlag(EffectFlags::VideoRandomCuts))
        {
            filters->Add("select='not(mod(n\,10))',setpts=N/FRAME_RATE/TB");
        }
        if (flags.HasFlag(EffectFlags::VideoShuffleFrames))
        {
            filters->Add("shuffleframes=1:1");
        }

        if (filters->Count == 0)
        {
            return String::Empty;
        }

        return String::Join(",", filters);
    }

    PipelineCommand^ FfmpegPipeline::BuildPreviewCommand(String^ inputPath, EffectFlags flags)
    {
        String^ filterGraph = BuildFilterGraph(flags);
        String^ arguments = String::Format("-autoexit -i \"{0}\"", inputPath);

        if (!String::IsNullOrWhiteSpace(filterGraph))
        {
            arguments += String::Format(" -vf \"{0}\"", filterGraph);
        }

        String^ executable = IsToolAvailable("ffplay") ? "ffplay" : "ffmpeg";

        if (executable->Equals("ffmpeg"))
        {
            arguments += " -f sdl \"preview\"";
        }

        return gcnew PipelineCommand(executable, arguments);
    }

    PipelineCommand^ FfmpegPipeline::BuildExportCommand(String^ concatListPath, String^ outputPath, EffectFlags flags)
    {
        String^ filterGraph = BuildFilterGraph(flags);
        String^ arguments = String::Format("-hide_banner -y -f concat -safe 0 -i \"{0}\"", concatListPath);

        if (!String::IsNullOrWhiteSpace(filterGraph))
        {
            arguments += String::Format(" -vf \"{0}\"", filterGraph);
        }

        arguments += String::Format(" -c:v libx264 -preset veryfast -crf 21 -c:a aac -b:a 192k \"{0}\"", outputPath);

        return gcnew PipelineCommand("ffmpeg", arguments);
    }

    void FfmpegPipeline::WriteConcatList(String^ outputPath, IEnumerable<String^>^ clipPaths)
    {
        if (String::IsNullOrWhiteSpace(outputPath))
        {
            throw gcnew ArgumentException("Output path cannot be empty.");
        }

        StringBuilder^ builder = gcnew StringBuilder();
        for each (String ^ clipPath in clipPaths)
        {
            if (String::IsNullOrWhiteSpace(clipPath))
            {
                continue;
            }

            String^ escaped = clipPath->Replace("'", "'\\''");
            builder->Append("file '")->Append(escaped)->Append("'\n");
        }

        File::WriteAllText(outputPath, builder->ToString(), Encoding::UTF8);
    }

    bool FfmpegPipeline::IsToolAvailable(String^ toolName)
    {
        if (String::IsNullOrWhiteSpace(toolName))
        {
            return false;
        }

        try
        {
            ProcessStartInfo^ info = gcnew ProcessStartInfo(toolName, "-version");
            info->CreateNoWindow = true;
            info->UseShellExecute = false;
            info->RedirectStandardOutput = true;
            info->RedirectStandardError = true;

            Process^ process = Process::Start(info);
            process->WaitForExit(1500);
            return process->ExitCode == 0;
        }
        catch (Exception^)
        {
            return false;
        }
    }

    String^ FfmpegPipeline::BuildEffectSummary(EffectFlags flags)
    {
        List<String^>^ names = gcnew List<String^>();

        if (flags.HasFlag(EffectFlags::AudioRandomSound)) names->Add("Random Sound");
        if (flags.HasFlag(EffectFlags::AudioMute)) names->Add("Mute");
        if (flags.HasFlag(EffectFlags::AudioSpeedUp)) names->Add("Speed Up");
        if (flags.HasFlag(EffectFlags::AudioSlowDown)) names->Add("Slow Down");
        if (flags.HasFlag(EffectFlags::AudioReverse)) names->Add("Reverse");
        if (flags.HasFlag(EffectFlags::AudioChorus)) names->Add("Chorus");
        if (flags.HasFlag(EffectFlags::AudioVibrato)) names->Add("Vibrato");
        if (flags.HasFlag(EffectFlags::AudioStutter)) names->Add("Stutter");
        if (flags.HasFlag(EffectFlags::AudioDance)) names->Add("Dance");
        if (flags.HasFlag(EffectFlags::AudioSquidward)) names->Add("Squidward");
        if (flags.HasFlag(EffectFlags::AudioSus)) names->Add("Sus");
        if (flags.HasFlag(EffectFlags::AudioLagfun)) names->Add("Lagfun");
        if (flags.HasFlag(EffectFlags::AudioLowHarmony)) names->Add("Low Harmony");
        if (flags.HasFlag(EffectFlags::AudioHighHarmony)) names->Add("High Harmony");
        if (flags.HasFlag(EffectFlags::AudioConfusion)) names->Add("Confusion");
        if (flags.HasFlag(EffectFlags::AudioRandomChords)) names->Add("Random Chords");
        if (flags.HasFlag(EffectFlags::AudioTrailingReverses)) names->Add("Trailing Reverses");
        if (flags.HasFlag(EffectFlags::AudioLowQualityMeme)) names->Add("Low Quality Meme");
        if (flags.HasFlag(EffectFlags::AudioCrust)) names->Add("Audio Crust");
        if (flags.HasFlag(EffectFlags::AudioPitchShiftLoop)) names->Add("Pitch-Shift Loop");
        if (flags.HasFlag(EffectFlags::AudioMashupMixing)) names->Add("Mashup Mixing");
        if (flags.HasFlag(EffectFlags::VideoInvert)) names->Add("Invert");
        if (flags.HasFlag(EffectFlags::VideoRainbow)) names->Add("Rainbow");
        if (flags.HasFlag(EffectFlags::VideoMirror)) names->Add("Mirror");
        if (flags.HasFlag(EffectFlags::VideoMirrorSymmetry)) names->Add("Mirror Symmetry");
        if (flags.HasFlag(EffectFlags::VideoScreenClip)) names->Add("Screen Clip");
        if (flags.HasFlag(EffectFlags::VideoOverlayImages)) names->Add("Overlay Images");
        if (flags.HasFlag(EffectFlags::VideoSpadinner)) names->Add("Spadinner");
        if (flags.HasFlag(EffectFlags::VideoSentenceMixing)) names->Add("Sentence Mixing");
        if (flags.HasFlag(EffectFlags::VideoShuffleFrames)) names->Add("Shuffle Frames");
        if (flags.HasFlag(EffectFlags::VideoFrameRateReduction)) names->Add("Framerate Reduction");
        if (flags.HasFlag(EffectFlags::VideoRandomCuts)) names->Add("Random Cuts");

        if (names->Count == 0)
        {
            return "None";
        }

        return String::Join(", ", names);
    }
}
