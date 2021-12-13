object frMain: TfrMain
  Left = 0
  Top = 0
  Caption = 'frMain'
  ClientHeight = 568
  ClientWidth = 1121
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pg: TPageControl
    Left = 0
    Top = 0
    Width = 1121
    Height = 568
    ActivePage = tsdebug
    Align = alClient
    TabOrder = 0
    ExplicitLeft = 424
    ExplicitTop = 208
    ExplicitWidth = 545
    ExplicitHeight = 313
    object tsmain: TTabSheet
      Caption = 'tsmain'
      ExplicitWidth = 281
      ExplicitHeight = 165
      object tl_main: TdxTileControl
        Left = 0
        Top = 0
        Width = 1113
        Height = 540
        ParentCustomHint = False
        OptionsBehavior.FocusItemOnCycle = False
        OptionsBehavior.ItemCheckMode = tcicmNone
        OptionsBehavior.ItemFocusMode = tcifmOuterFrame
        OptionsBehavior.ItemHotTrackHighlightColor = 0
        OptionsBehavior.ItemHotTrackMode = tcihtmNone
        OptionsBehavior.ItemMoving = False
        OptionsBehavior.ItemPressAnimation = False
        OptionsBehavior.ScrollMode = smDefault
        OptionsView.CenterContentHorz = True
        OptionsView.CenterContentVert = True
        OptionsView.FixedIndentHorz = True
        OptionsView.FixedIndentVert = True
        OptionsView.GroupBlockMaxColumnCount = 4
        OptionsView.ItemHeight = 130
        OptionsView.ItemIndent = 20
        OptionsView.ItemWidth = 130
        Style.GradientBeginColor = clWhite
        Style.GradientEndColor = clWhite
        TabOrder = 0
        ExplicitLeft = 448
        ExplicitTop = 200
        ExplicitWidth = 400
        ExplicitHeight = 300
        object dxTileControl1Group1: TdxTileControlGroup
          Index = 0
        end
        object dxTileControl1Item1: TdxTileControlItem
          GroupIndex = 0
          IndexInGroup = 0
          Style.GradientBeginColor = clWhite
          Style.GradientEndColor = clWhite
          OptionsAnimate.AnimateText = False
          Text1.AssignedValues = []
          Text2.AssignedValues = []
          Text3.AssignedValues = []
          Text4.AssignedValues = []
        end
        object dxTileControl1Item2: TdxTileControlItem
          GroupIndex = 0
          IndexInGroup = 1
          Style.GradientBeginColor = clWhite
          Style.GradientEndColor = clWhite
          OptionsAnimate.AnimateText = False
          Text1.AssignedValues = []
          Text2.AssignedValues = []
          Text3.AssignedValues = []
          Text4.AssignedValues = []
        end
        object dxTileControl1Item3: TdxTileControlItem
          GroupIndex = 0
          IndexInGroup = 2
          Style.GradientBeginColor = clWhite
          Style.GradientEndColor = clWhite
          OptionsAnimate.AnimateText = False
          Text1.AssignedValues = []
          Text2.AssignedValues = []
          Text3.AssignedValues = []
          Text4.AssignedValues = []
        end
        object dxTileControl1Item4: TdxTileControlItem
          GroupIndex = 0
          IndexInGroup = 3
          Style.GradientBeginColor = clWhite
          Style.GradientEndColor = clWhite
          OptionsAnimate.AnimateText = False
          Text1.AssignedValues = []
          Text2.AssignedValues = []
          Text3.AssignedValues = []
          Text4.AssignedValues = []
        end
      end
    end
    object tssett: TTabSheet
      Caption = 'tssett'
      ImageIndex = 1
      ExplicitWidth = 281
      ExplicitHeight = 165
    end
    object tscontroll: TTabSheet
      Caption = 'tscontroll'
      ImageIndex = 2
      ExplicitWidth = 281
      ExplicitHeight = 165
    end
    object tstodo: TTabSheet
      Caption = 'tstodo'
      ImageIndex = 3
      ExplicitWidth = 281
      ExplicitHeight = 165
    end
    object tsdebug: TTabSheet
      Caption = 'tsdebug'
      ImageIndex = 4
      ExplicitLeft = 8
      ExplicitTop = 28
      ExplicitWidth = 1105
      ExplicitHeight = 542
      object Label1: TLabel
        Left = 16
        Top = 13
        Width = 51
        Height = 13
        Caption = 'ping range'
      end
      object Label3: TLabel
        Left = 303
        Top = 13
        Width = 45
        Height = 13
        Caption = 'device ok'
      end
      object m_ip: TMemo
        Left = 16
        Top = 32
        Width = 281
        Height = 497
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI Semibold'
        Font.Style = [fsBold]
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
      end
      object m_dev: TMemo
        Left = 303
        Top = 32
        Width = 274
        Height = 497
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Segoe UI Semibold'
        Font.Style = [fsBold]
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 1
      end
    end
  end
end
