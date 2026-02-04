Imports System

Public Enum ProjectType
    Generic
    YtpTennis
    CollabEntry
    YtpMv
End Enum

Public Enum YtpStyle
    Classic2007To2012
    Modern2013To2021
    AdvanceExperimental
End Enum

Public Enum MixerChannel
    Master
    Aux1
    Aux2
    Midi1
    Midi2
    Midi3
End Enum

Public Class SourceEntry
    Public Property PathOrUrl As String
    Public Property MediaType As String
    Public Property Tags As String

    Public Sub New(value As String, mediaType As String, Optional tags As String = "")
        PathOrUrl = value
        Me.MediaType = mediaType
        Me.Tags = tags
    End Sub
End Class

Public Class ProjectModel
    Public Property Title As String = "Untitled YTP"
    Public Property ProjectType As ProjectType = ProjectType.Generic
    Public Property YtpStyle As YtpStyle = YtpStyle.Classic2007To2012
    Public Property MinStreamSeconds As Integer = 2
    Public Property MaxStreamSeconds As Integer = 12
    Public Property ClipCount As Integer = 120
    Public Property OutputFormat As String = "mp4"
    Public Property RecallEnabled As Boolean = False

    Public ReadOnly Property Sources As List(Of SourceEntry) = New()
    Public ReadOnly Property OnlineUrls As List(Of String) = New()
    Public ReadOnly Property Effects As Dictionary(Of String, Boolean) = New()
    Public ReadOnly Property MixerChannels As List(Of MixerChannel) = New() From {
        MixerChannel.Master,
        MixerChannel.Aux1,
        MixerChannel.Aux2,
        MixerChannel.Midi1,
        MixerChannel.Midi2,
        MixerChannel.Midi3
    }

    Public ReadOnly Property AssetFolders As Dictionary(Of String, String) = New()

    Public Sub New()
        Dim defaults = New Dictionary(Of String, Boolean) From {
            {"Audio: Random sound", False},
            {"Audio: Mute", False},
            {"Audio: Speed up", False},
            {"Audio: Slow down", False},
            {"Audio: Reverse", False},
            {"Audio: Chorus", False},
            {"Audio: Vibrato", False},
            {"Audio: Stutter", False},
            {"Audio: Dance", False},
            {"Audio: Squidward", False},
            {"Audio: Sus", False},
            {"Audio: Lagfun", False},
            {"Audio: Low harmony", False},
            {"Audio: High harmony", False},
            {"Audio: Confusion", False},
            {"Audio: Random chords", False},
            {"Audio: Trailing reverses", False},
            {"Audio: Low quality meme", False},
            {"Audio: Audio crust", False},
            {"Audio: Pitch shift loop", False},
            {"Audio: Mashup mixing", False},
            {"Video: Invert", False},
            {"Video: Rainbow", False},
            {"Video: Mirror", False},
            {"Video: Mirror symmetry", False},
            {"Video: Screen clip", False},
            {"Video: Overlay images", False},
            {"Video: Spadinner", False},
            {"Video: Sentence mixing", False},
            {"Video: Shuffle frames", False},
            {"Video: Loop frames", False},
            {"Video: Framerate reduction", False},
            {"Video: Random cuts", False},
            {"Video: Speed loop boost", False},
            {"Video: Scramble", False},
            {"Assets: Adverts", False},
            {"Assets: Errors", False},
            {"Assets: Images", False},
            {"Assets: Meme sounds", False},
            {"Assets: Memes", False},
            {"Assets: Overlay videos", False},
            {"Assets: Sounds", False}
        }

        For Each item In defaults
            Effects(item.Key) = item.Value
        Next

        AssetFolders("images") = String.Empty
        AssetFolders("memes") = String.Empty
        AssetFolders("meme_sounds") = String.Empty
        AssetFolders("sounds") = String.Empty
        AssetFolders("overlay_videos") = String.Empty
        AssetFolders("adverts") = String.Empty
        AssetFolders("errors") = String.Empty
    End Sub
End Class
