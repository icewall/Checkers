object Form1: TForm1
  Left = 411
  Top = 56
  Caption = 'IceCheckers'
  ClientHeight = 538
  ClientWidth = 584
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClick = FormClick
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 421
    Width = 584
    Height = 117
    Align = alBottom
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object CheckBox1: TCheckBox
    Left = 472
    Top = 360
    Width = 97
    Height = 17
    Caption = 'AlphaBeta'
    Checked = True
    State = cbChecked
    TabOrder = 1
  end
end
