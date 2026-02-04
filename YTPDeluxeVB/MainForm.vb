Imports System
Imports System.IO
Imports System.Text
Imports System.Windows.Forms

Public Class MainForm
    Inherits Form

    Private ReadOnly _model As New ProjectModel()
    Private ReadOnly _runner As New FfmpegRunner()

    Private ReadOnly _sourcesView As New ListView()
    Private ReadOnly _urlsList As New ListBox()
    Private ReadOnly _effectsList As New CheckedListBox()
    Private ReadOnly _projectTypeCombo As New ComboBox()
    Private ReadOnly _styleCombo As New ComboBox()
    Private ReadOnly _outputFormatCombo As New ComboBox()
    Private ReadOnly _minStream As New NumericUpDown()
    Private ReadOnly _maxStream As New NumericUpDown()
    Private ReadOnly _clipCount As New NumericUpDown()
    Private ReadOnly _titleBox As New TextBox()
    Private ReadOnly _logBox As New TextBox()
    Private ReadOnly _assetGrid As New DataGridView()
    Private ReadOnly _previewButton As New Button()
    Private ReadOnly _exportButton As New Button()

    Public Sub New()
        Text = "YTP Deluxe - VB WinForms"
        Width = 1280
        Height = 860
        MinimumSize = New Drawing.Size(1100, 760)

        InitializeLayout()
        LoadModel()
    End Sub

    Private Sub InitializeLayout()
        Dim mainLayout As New TableLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .RowCount = 2,
            .ColumnCount = 2
        }
        mainLayout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 60))
        mainLayout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 40))
        mainLayout.RowStyles.Add(New RowStyle(SizeType.Percent, 70))
        mainLayout.RowStyles.Add(New RowStyle(SizeType.Percent, 30))

        Dim sourcesGroup = BuildSourcesGroup()
        Dim settingsGroup = BuildSettingsGroup()
        Dim effectsGroup = BuildEffectsGroup()
        Dim outputGroup = BuildOutputGroup()

        mainLayout.Controls.Add(sourcesGroup, 0, 0)
        mainLayout.Controls.Add(settingsGroup, 1, 0)
        mainLayout.Controls.Add(effectsGroup, 0, 1)
        mainLayout.Controls.Add(outputGroup, 1, 1)

        Controls.Add(mainLayout)
    End Sub

    Private Function BuildSourcesGroup() As GroupBox
        Dim group As New GroupBox() With {
            .Text = "Source Explorer + Online URLs",
            .Dock = DockStyle.Fill
        }

        Dim layout As New TableLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .RowCount = 2,
            .ColumnCount = 2
        }
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 70))
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 30))
        layout.RowStyles.Add(New RowStyle(SizeType.Percent, 80))
        layout.RowStyles.Add(New RowStyle(SizeType.Percent, 20))

        _sourcesView.View = View.Details
        _sourcesView.Columns.Add("Source", 380)
        _sourcesView.Columns.Add("Type", 120)
        _sourcesView.Columns.Add("Tags", 160)
        _sourcesView.FullRowSelect = True
        _sourcesView.Dock = DockStyle.Fill

        Dim urlPanel As New Panel() With {.Dock = DockStyle.Fill}
        _urlsList.Dock = DockStyle.Fill
        urlPanel.Controls.Add(_urlsList)

        Dim buttonPanel As New FlowLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .FlowDirection = FlowDirection.LeftToRight
        }

        Dim addMediaButton As New Button() With {.Text = "Add Media"}
        AddHandler addMediaButton.Click, AddressOf AddMediaClicked
        Dim addFolderButton As New Button() With {.Text = "Add Folder"}
        AddHandler addFolderButton.Click, AddressOf AddFolderClicked
        Dim addUrlButton As New Button() With {.Text = "Add URL"}
        AddHandler addUrlButton.Click, AddressOf AddUrlClicked
        Dim removeButton As New Button() With {.Text = "Remove Selected"}
        AddHandler removeButton.Click, AddressOf RemoveSelectedClicked

        buttonPanel.Controls.Add(addMediaButton)
        buttonPanel.Controls.Add(addFolderButton)
        buttonPanel.Controls.Add(addUrlButton)
        buttonPanel.Controls.Add(removeButton)

        layout.Controls.Add(_sourcesView, 0, 0)
        layout.SetColumnSpan(_sourcesView, 2)
        layout.Controls.Add(urlPanel, 0, 1)
        layout.Controls.Add(buttonPanel, 1, 1)

        group.Controls.Add(layout)
        Return group
    End Function

    Private Function BuildSettingsGroup() As GroupBox
        Dim group As New GroupBox() With {
            .Text = "Project Settings + Assets",
            .Dock = DockStyle.Fill
        }

        Dim layout As New TableLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .RowCount = 6,
            .ColumnCount = 2
        }
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 40))
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 60))

        layout.Controls.Add(New Label() With {.Text = "Title", .AutoSize = True}, 0, 0)
        _titleBox.Dock = DockStyle.Fill
        layout.Controls.Add(_titleBox, 1, 0)

        layout.Controls.Add(New Label() With {.Text = "Project Type", .AutoSize = True}, 0, 1)
        _projectTypeCombo.Dock = DockStyle.Fill
        _projectTypeCombo.DropDownStyle = ComboBoxStyle.DropDownList
        layout.Controls.Add(_projectTypeCombo, 1, 1)

        layout.Controls.Add(New Label() With {.Text = "YTP Style", .AutoSize = True}, 0, 2)
        _styleCombo.Dock = DockStyle.Fill
        _styleCombo.DropDownStyle = ComboBoxStyle.DropDownList
        layout.Controls.Add(_styleCombo, 1, 2)

        layout.Controls.Add(New Label() With {.Text = "Stream Seconds (Min/Max)", .AutoSize = True}, 0, 3)
        Dim streamPanel As New FlowLayoutPanel() With {.Dock = DockStyle.Fill}
        _minStream.Minimum = 1
        _minStream.Maximum = 600
        _maxStream.Minimum = 1
        _maxStream.Maximum = 600
        streamPanel.Controls.Add(_minStream)
        streamPanel.Controls.Add(New Label() With {.Text = "to", .AutoSize = True, .Padding = New Padding(6, 6, 6, 0)})
        streamPanel.Controls.Add(_maxStream)
        layout.Controls.Add(streamPanel, 1, 3)

        layout.Controls.Add(New Label() With {.Text = "Clip Count", .AutoSize = True}, 0, 4)
        _clipCount.Minimum = 1
        _clipCount.Maximum = 70000
        _clipCount.Dock = DockStyle.Left
        layout.Controls.Add(_clipCount, 1, 4)

        layout.Controls.Add(New Label() With {.Text = "Asset Folders", .AutoSize = True}, 0, 5)
        _assetGrid.Dock = DockStyle.Fill
        _assetGrid.ColumnCount = 2
        _assetGrid.Columns(0).Name = "Asset"
        _assetGrid.Columns(1).Name = "Folder"
        _assetGrid.AllowUserToAddRows = False
        _assetGrid.AllowUserToDeleteRows = False
        _assetGrid.RowHeadersVisible = False
        layout.Controls.Add(_assetGrid, 1, 5)

        group.Controls.Add(layout)
        Return group
    End Function

    Private Function BuildEffectsGroup() As GroupBox
        Dim group As New GroupBox() With {
            .Text = "Effects + Mixer Channels",
            .Dock = DockStyle.Fill
        }

        Dim layout As New TableLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .RowCount = 1,
            .ColumnCount = 2
        }
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 70))
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 30))

        _effectsList.Dock = DockStyle.Fill

        Dim mixerBox As New ListBox() With {.Dock = DockStyle.Fill}
        For Each channel In _model.MixerChannels
            mixerBox.Items.Add(channel.ToString())
        Next

        layout.Controls.Add(_effectsList, 0, 0)
        layout.Controls.Add(mixerBox, 1, 0)

        group.Controls.Add(layout)
        Return group
    End Function

    Private Function BuildOutputGroup() As GroupBox
        Dim group As New GroupBox() With {
            .Text = "Preview + Export",
            .Dock = DockStyle.Fill
        }

        Dim layout As New TableLayoutPanel() With {
            .Dock = DockStyle.Fill,
            .RowCount = 3,
            .ColumnCount = 2
        }
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 30))
        layout.ColumnStyles.Add(New ColumnStyle(SizeType.Percent, 70))

        layout.Controls.Add(New Label() With {.Text = "Output Format", .AutoSize = True}, 0, 0)
        _outputFormatCombo.Dock = DockStyle.Fill
        _outputFormatCombo.DropDownStyle = ComboBoxStyle.DropDownList
        layout.Controls.Add(_outputFormatCombo, 1, 0)

        Dim buttonPanel As New FlowLayoutPanel() With {.Dock = DockStyle.Fill}
        _previewButton.Text = "Preview"
        _exportButton.Text = "Export"
        AddHandler _previewButton.Click, AddressOf PreviewClicked
        AddHandler _exportButton.Click, AddressOf ExportClicked
        buttonPanel.Controls.Add(_previewButton)
        buttonPanel.Controls.Add(_exportButton)
        layout.Controls.Add(buttonPanel, 1, 1)

        _logBox.Dock = DockStyle.Fill
        _logBox.Multiline = True
        _logBox.ScrollBars = ScrollBars.Vertical
        _logBox.ReadOnly = True
        _logBox.BackColor = Drawing.Color.Black
        _logBox.ForeColor = Drawing.Color.LightGreen

        layout.Controls.Add(New Label() With {.Text = "Pipeline Log", .AutoSize = True}, 0, 2)
        layout.Controls.Add(_logBox, 1, 2)

        group.Controls.Add(layout)
        Return group
    End Function

    Private Sub LoadModel()
        _titleBox.Text = _model.Title
        _projectTypeCombo.Items.AddRange([Enum].GetNames(GetType(ProjectType)))
        _styleCombo.Items.AddRange(New String() {
            "YTP 2007-2012 (classic)",
            "YTP 2013-2021 (modern)",
            "YTP Advance (experimental)"
        })
        _outputFormatCombo.Items.AddRange(New String() {"mp4", "wmv", "avi", "mkv"})

        _projectTypeCombo.SelectedIndex = CInt(_model.ProjectType)
        _styleCombo.SelectedIndex = CInt(_model.YtpStyle)
        _outputFormatCombo.SelectedItem = _model.OutputFormat

        _minStream.Value = _model.MinStreamSeconds
        _maxStream.Value = _model.MaxStreamSeconds
        _clipCount.Value = _model.ClipCount

        For Each effect In _model.Effects
            _effectsList.Items.Add(effect.Key, effect.Value)
        Next

        _assetGrid.Rows.Clear()
        For Each asset In _model.AssetFolders
            _assetGrid.Rows.Add(asset.Key, asset.Value)
        Next

        AppendLog("YTP Deluxe VB project loaded.")
        AppendLog($"Preview engine: {(If(_runner.CanPreview(), "FFplay/FFmpeg", "Missing FFplay/FFmpeg"))}")
    End Sub

    Private Sub AddMediaClicked(sender As Object, e As EventArgs)
        Dim dialog As New OpenFileDialog() With {
            .Filter = "Media files|*.mp4;*.wmv;*.avi;*.mkv;*.mp3;*.wav;*.ogg;*.png;*.jpg;*.jpeg;*.webp;*.gif;*.xm;*.mod;*.it|All files|*.*",
            .Multiselect = True
        }

        If dialog.ShowDialog() = DialogResult.OK Then
            For Each file In dialog.FileNames
                Dim mediaType = DetectMediaType(file)
                _model.Sources.Add(New SourceEntry(file, mediaType))
                Dim item = New ListViewItem(New String() {file, mediaType, ""})
                _sourcesView.Items.Add(item)
            Next
            AppendLog($"Added {dialog.FileNames.Length} sources.")
        End If
    End Sub

    Private Sub AddFolderClicked(sender As Object, e As EventArgs)
        Dim dialog As New FolderBrowserDialog()
        If dialog.ShowDialog() = DialogResult.OK Then
            Dim files = Directory.GetFiles(dialog.SelectedPath)
            Dim addedCount = 0
            For Each file In files
                Dim mediaType = DetectMediaType(file)
                If Not String.Equals(mediaType, "Unknown", StringComparison.OrdinalIgnoreCase) Then
                    _model.Sources.Add(New SourceEntry(file, mediaType))
                    Dim item = New ListViewItem(New String() {file, mediaType, ""})
                    _sourcesView.Items.Add(item)
                    addedCount += 1
                End If
            Next
            AppendLog($"Added {addedCount} sources from folder.")
        End If
    End Sub

    Private Sub AddUrlClicked(sender As Object, e As EventArgs)
        Dim input = InputBox("Enter media URL:", "Add Online URL")
        If Not String.IsNullOrWhiteSpace(input) Then
            _model.OnlineUrls.Add(input)
            _urlsList.Items.Add(input)
            AppendLog("Registered online URL for later download.")
        End If
    End Sub

    Private Sub RemoveSelectedClicked(sender As Object, e As EventArgs)
        For Each item As ListViewItem In _sourcesView.SelectedItems
            Dim path = item.SubItems(0).Text
            Dim entry = _model.Sources.Find(Function(x) x.PathOrUrl = path)
            If entry IsNot Nothing Then
                _model.Sources.Remove(entry)
            End If
            _sourcesView.Items.Remove(item)
        Next

        Dim selectedUrl = TryCast(_urlsList.SelectedItem, String)
        If selectedUrl IsNot Nothing Then
            _model.OnlineUrls.Remove(selectedUrl)
            _urlsList.Items.Remove(selectedUrl)
        End If
    End Sub

    Private Sub PreviewClicked(sender As Object, e As EventArgs)
        Dim selected = GetPrimarySource()
        If selected Is Nothing Then
            AppendLog("Select a source to preview.")
            Return
        End If

        Dim command = _runner.PreviewCommand(selected.PathOrUrl)
        If String.IsNullOrWhiteSpace(command) Then
            AppendLog("FFplay/FFmpeg not found in PATH.")
            Return
        End If

        AppendLog($"Preview: {command}")
        LaunchProcess(command)
    End Sub

    Private Sub ExportClicked(sender As Object, e As EventArgs)
        Dim selectedSources = _model.Sources.Select(Function(x) x.PathOrUrl).ToList()
        If selectedSources.Count = 0 Then
            AppendLog("Add sources before exporting.")
            Return
        End If

        Dim dialog As New SaveFileDialog() With {
            .Filter = "Video files|*.mp4;*.wmv;*.avi;*.mkv",
            .FileName = "ytp_deluxe_export"
        }

        If dialog.ShowDialog() <> DialogResult.OK Then
            Return
        End If

        SyncFormToModel()
        Dim filterGraph = BuildFilterGraph()
        Dim outputFormat = _model.OutputFormat
        Dim command = _runner.ExportCommand(selectedSources, dialog.FileName, filterGraph, outputFormat)
        AppendLog($"Export: {command}")
        LaunchProcess(command)
    End Sub

    Private Sub SyncFormToModel()
        _model.Title = _titleBox.Text
        _model.ProjectType = CType(_projectTypeCombo.SelectedIndex, ProjectType)
        _model.YtpStyle = CType(_styleCombo.SelectedIndex, YtpStyle)
        _model.MinStreamSeconds = CInt(_minStream.Value)
        _model.MaxStreamSeconds = CInt(_maxStream.Value)
        _model.ClipCount = CInt(_clipCount.Value)
        _model.OutputFormat = _outputFormatCombo.SelectedItem.ToString()

        For index = 0 To _effectsList.Items.Count - 1
            Dim key = _effectsList.Items(index).ToString()
            _model.Effects(key) = _effectsList.GetItemChecked(index)
        Next

        For Each row As DataGridViewRow In _assetGrid.Rows
            Dim asset = TryCast(row.Cells(0).Value, String)
            Dim folder = TryCast(row.Cells(1).Value, String)
            If Not String.IsNullOrWhiteSpace(asset) Then
                _model.AssetFolders(asset) = If(folder, String.Empty)
            End If
        Next
    End Sub

    Private Function BuildFilterGraph() As String
        Dim filters As New List(Of String)()

        If _model.Effects("Video: Invert") Then
            filters.Add("negate")
        End If
        If _model.Effects("Video: Rainbow") Then
            filters.Add("hue=s=2")
        End If
        If _model.Effects("Video: Mirror") Then
            filters.Add("hflip")
        End If
        If _model.Effects("Video: Mirror symmetry") Then
            filters.Add("split [a][b]; [b] hflip [b2]; [a][b2] hstack")
        End If
        If _model.Effects("Video: Framerate reduction") Then
            filters.Add("fps=12")
        End If
        If _model.Effects("Video: Scramble") Then
            filters.Add("shuffleframes")
        End If
        If _model.Effects("Video: Random cuts") Then
            filters.Add("select='not(mod(n,5))',setpts=N/FRAME_RATE/TB")
        End If

        If _model.Effects("Audio: Reverse") Then
            filters.Add("areverse")
        End If
        If _model.Effects("Audio: Chorus") Then
            filters.Add("chorus=0.7:0.9:55:0.4:0.25:2")
        End If
        If _model.Effects("Audio: Vibrato") Then
            filters.Add("vibrato=f=6")
        End If
        If _model.Effects("Audio: Stutter") Then
            filters.Add("aselect='not(mod(n,2))',asetpts=N/SR/TB")
        End If
        If _model.Effects("Audio: Speed up") Then
            filters.Add("atempo=1.3")
        End If
        If _model.Effects("Audio: Slow down") Then
            filters.Add("atempo=0.8")
        End If
        If _model.Effects("Audio: Pitch shift loop") Then
            filters.Add("rubberband=pitch=1.2")
        End If

        Return String.Join(",", filters)
    End Function

    Private Function DetectMediaType(file As String) As String
        Dim ext = Path.GetExtension(file).ToLowerInvariant()
        Select Case ext
            Case ".mp4", ".wmv", ".avi", ".mkv"
                Return "Video"
            Case ".png", ".jpg", ".jpeg", ".webp", ".gif"
                Return "Image/GIF"
            Case ".mp3", ".wav", ".ogg"
                Return "Audio"
            Case ".xm", ".mod", ".it"
                Return "Tracker"
            Case Else
                Return "Unknown"
        End Select
    End Function

    Private Function GetPrimarySource() As SourceEntry
        If _sourcesView.SelectedItems.Count > 0 Then
            Dim path = _sourcesView.SelectedItems(0).SubItems(0).Text
            Return _model.Sources.Find(Function(x) x.PathOrUrl = path)
        End If
        If _model.Sources.Count > 0 Then
            Return _model.Sources(0)
        End If
        Return Nothing
    End Function

    Private Sub AppendLog(message As String)
        _logBox.AppendText($"[{DateTime.Now:HH:mm:ss}] {message}{Environment.NewLine}")
    End Sub

    Private Sub LaunchProcess(command As String)
        Try
            Dim info As New ProcessStartInfo("cmd.exe", $"/c {command}") With {
                .CreateNoWindow = False,
                .UseShellExecute = True
            }
            Process.Start(info)
        Catch ex As Exception
            AppendLog($"Failed to start process: {ex.Message}")
        End Try
    End Sub
End Class
