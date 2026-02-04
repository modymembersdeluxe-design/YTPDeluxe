#pragma once

#include "ProjectModel.h"

using namespace System;
using namespace System::Collections::Generic;

namespace YTPDeluxe
{
    public ref class PipelineCommand
    {
    public:
        PipelineCommand(String^ executable, String^ arguments)
            : Executable(executable), Arguments(arguments)
        {
        }

        property String^ Executable;
        property String^ Arguments;
    };

    public ref class FfmpegPipeline
    {
    public:
        static String^ BuildFilterGraph(EffectFlags flags);
        static PipelineCommand^ BuildPreviewCommand(String^ inputPath, EffectFlags flags);
        static PipelineCommand^ BuildExportCommand(String^ concatListPath, String^ outputPath, EffectFlags flags);
        static void WriteConcatList(String^ outputPath, IEnumerable<String^>^ clipPaths);
        static bool IsToolAvailable(String^ toolName);
        static String^ BuildEffectSummary(EffectFlags flags);
    };
}
