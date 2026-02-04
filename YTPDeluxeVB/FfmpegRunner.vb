Imports System
Imports System.Diagnostics
Imports System.IO
Imports System.Text

Public Class FfmpegRunner
    Public ReadOnly Property FfmpegPath As String
    Public ReadOnly Property FfplayPath As String

    Public Sub New()
        FfmpegPath = FindExecutable("ffmpeg")
        FfplayPath = FindExecutable("ffplay")
    End Sub

    Public Function CanPreview() As Boolean
        Return Not String.IsNullOrWhiteSpace(FfplayPath) OrElse Not String.IsNullOrWhiteSpace(FfmpegPath)
    End Function

    Public Function PreviewCommand(inputPath As String) As String
        If Not String.IsNullOrWhiteSpace(FfplayPath) Then
            Return $"\"{FfplayPath}\" -autoexit -i \"{inputPath}\""
        End If
        If Not String.IsNullOrWhiteSpace(FfmpegPath) Then
            Return $"\"{FfmpegPath}\" -i \"{inputPath}\" -f null -"
        End If
        Return ""
    End Function

    Public Function ExportCommand(inputs As IReadOnlyList(Of String), outputPath As String, filters As String, outputFormat As String) As String
        Dim inputArguments = New StringBuilder()
        For Each input In inputs
            inputArguments.Append($" -i \"{input}\"")
        Next

        Dim filterArgument = String.Empty
        If Not String.IsNullOrWhiteSpace(filters) Then
            filterArgument = $" -filter_complex \"{filters}\""
        End If

        Dim formatArgument = String.Empty
        If Not String.IsNullOrWhiteSpace(outputFormat) Then
            formatArgument = $" -f {outputFormat}"
        End If

        Return $"\"{FfmpegPath}\"{inputArguments}{filterArgument} -shortest -y{formatArgument} \"{outputPath}\""
    End Function

    Public Function WriteConcatList(paths As IReadOnlyList(Of String), destinationPath As String) As Boolean
        Try
            Dim directory = Path.GetDirectoryName(destinationPath)
            If Not String.IsNullOrWhiteSpace(directory) Then
                Directory.CreateDirectory(directory)
            End If

            Using writer As New StreamWriter(destinationPath, False, Encoding.UTF8)
                For Each item In paths
                    Dim sanitized = item.Replace("'", "'\\''")
                    writer.WriteLine($"file '{sanitized}'")
                Next
            End Using
            Return True
        Catch
            Return False
        End Try
    End Function

    Private Shared Function FindExecutable(name As String) As String
        Dim extensions = New List(Of String) From {".exe", ""}
        Dim pathEnv = Environment.GetEnvironmentVariable("PATH")
        If String.IsNullOrWhiteSpace(pathEnv) Then
            Return String.Empty
        End If

        For Each folder In pathEnv.Split(Path.PathSeparator)
            For Each ext In extensions
                Dim candidate = Path.Combine(folder, name & ext)
                If File.Exists(candidate) Then
                    Return candidate
                End If
            Next
        Next

        Return String.Empty
    End Function
End Class
