Imports System.ComponentModel
Imports System.Configuration.Install

Public Class Installer1

    Public Sub New()
        MyBase.New()

        'This call is required by the Component Designer.
        InitializeComponent()

        'Add initialization code after the call to InitializeComponent

    End Sub

    Public Overrides Sub Install(ByVal stateSaver As System.Collections.IDictionary)
        MyBase.Install(stateSaver)

        'Get Folder Path
        Dim mySrcDir As String = Me.Context.Parameters.Item("SrcDir")
        Dim myTarDir As String = Me.Context.Parameters.Item("TarDir")

        Dim mySrcDir2 As String
        Dim myTarDir2 As String

        mySrcDir2 = mySrcDir + "\STH OCC"
        myTarDir2 = myTarDir + "\STH OCC"

        'Copy Folder From SrcDir to TarDir
        My.Computer.FileSystem.CopyDirectory(mySrcDir2, myTarDir2, True)

    End Sub

End Class
