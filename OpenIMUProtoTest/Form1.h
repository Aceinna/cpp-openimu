#pragma once
#include "protocol.hpp"
#include "XbowPacket.hpp"
#include "XbowCmdResponsePkt.hpp"
#include "OpenIMUCmdResponsePkt.hpp"
#include "XbowsensorScaledPkt.hpp"
#include "xbowVersionDataPkt.hpp"
#include "xbowTestPkt.hpp"
#include "XbowAnglePkt.hpp"
#include "xassert.h"
#include "StreamBuffer.hpp""
#include "time.h"
#include "math.h"


namespace ProbeCon {
	using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Reflection;
	using namespace System::Threading;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	public:
		Form1(void) : _protocol(NULL)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			Type^ clsType = Form1::typeid;
			Assembly^ assy = clsType->Assembly;
			_version = assy->GetName()->Version->ToString();
			Text = Text + " (" + _version + ")";
			versionToolStripMenuItem->Text = "version " + _version; 
			_fileOpened = false;
			_errorCounter = 0;
			_crcErrorCounter = 0;
			_cmdCounter = 0;
			_pingFrequency = 0;
			_pingRunning   = false;
			_prevMaxTripTime = 0;
			_calData = new double[1000];
			_calDataLen = 0;
		}
		
		void Form1::ThreadFunction();
		void Form1::stopPing();


//		void ThreadFunction()
//		{
//			MyThreadClass^ myThreadClassObject = gcnew MyThreadClass(this);
//		myThreadClassObject->Run();
//		}

		void setProtocol(TestProto *protocol){
			_protocol = protocol;
		}

		bool isThreadRunning(){
			return _pingRunning;
		}

		int pingFrequency(){
			return _pingFrequency;
		}


		void setComPorts(int *portArray, int portCount){
			char portName[20];
			for(int i=0; i<portCount; i++){
				sprintf(portName,"COM%d", portArray[i]);
				comboBox1->Items->Add(gcnew System::String(portName));
			}
		}
		
		uint8_t *getMessage(Button^ pButton, SerialStream * stream, uint32_t sync){
			uint8_t *msg = NULL;
			int crc = 0;
			pButton->Enabled = false;
			msg = _protocol->getMessage(stream, sync, &crc);
			pButton->Enabled = true;
			if (!msg || crc){
				_errorCounter++;
				String^ info = "Timeout Errors : " + _errorCounter.ToString() + "  CRC errors : " + _crcErrorCounter.ToString();
				errorLabel->Text = info;
				errorLabel->Visible = true;
			}
			_cmdCounter++;
			return msg;
		}

		void showBits(uint16_t bits){
/*
			if (bits & 0x0001){
				Bit0->Checked = true;
			}
			else {
				Bit0->Checked = false;
			}
			//**************
			if (bits & 0x0002){
				Bit1->Checked = true;
			}
			else {
				Bit1->Checked = false;
			}
			//**************
			if (bits & 0x0004){
				Bit2->Checked = true;
			}
			else {
				Bit2->Checked = false;
			}
			//**************
			if (bits & 0x0008){
				Bit3->Checked = true;
			}
			else {
				Bit3->Checked = false;
			}
			//**************
			if (bits & 0x0010){
				Bit4->Checked = true;
			}
			else {
				Bit4->Checked = false;
			}
			//**************
			if (bits & 0x0020){
				Bit5->Checked = true;
			}
			else {
				Bit5->Checked = false;
			}
			//**************
			if (bits & 0x0040){
				Bit6->Checked = true;
			}
			else {
				Bit6->Checked = false;
			}
			//**************
			if (bits & 0x0080){
				Bit7->Checked = true;
			}
			else {
				Bit7->Checked = false;
			}
			//**************
			if (bits & 0x0100){
				Bit8->Checked = true;
			}
			else {
				Bit8->Checked = false;
			}
			//**************
			if (bits & 0x0200){
				Bit9->Checked = true;
			}
			else {
				Bit9->Checked = false;
			}
			//**************
			if (bits & 0x0400){
				Bit10->Checked = true;
			}
			else {
				Bit10->Checked = false;
			}
			//**************
			if (bits & 0x0800){
				Bit11->Checked = true;
			}
			else {
				Bit11->Checked = false;
			}
			//**************
			if (bits & 0x1000){
				Bit12->Checked = true;
			}
			else {
				Bit12->Checked = false;
			}
	*/
		}
		void showStatus(uint16_t bits){
/*
			//**************
			StatusMode->Text = (bits & 0x07).ToString();
			if (bits & 0x0008){
				StatusLed->Checked = true;
			}
			else {
				StatusLed->Checked = false;
			}
			//**************
			StatusAlign->Text = ((bits >> 4) & 0x03).ToString();
			//**************
			if (bits & 0x0040){
				StatusNfKo->Checked = true;
			}
			else {
				StatusNfKo->Checked = false;
			}
			//**************
			if (bits & 0x0080){
				StatusBusy->Checked = true;
			}
			else {
				StatusBusy->Checked = false;
			}
			//**************
			StatusBattery->Text = ((bits >> 8) & 0x1f).ToString();
			//**************
			if (bits & 0x2000){
				StatusPwr->Checked = true;
			}
			else {
				StatusPwr->Checked = false;
			}
			//**************
			if (bits & 0x4000){
				StatusCmdOk->Checked = true;
			}
			else {
				StatusCmdOk->Checked = false;
			}
			//**************
			if (bits & 0x8000){
				StatusMsgKo->Checked = true;
			}
			else {
				StatusMsgKo->Checked = false;
			}
*/		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

			 bool			_fileOpened;
			 int			_errorCounter;
			 int			_crcErrorCounter;
			 int			_cmdCounter;
			 TestProto      *_protocol;
			 SerialStream	*_stream;
			 System::String^ _version;
			 System::String^ _openFileName;
			 System::String^ _recentFile;
			 Thread^		 _myThread;
			 int			 _pingFrequency;
			 int			 _pingRunning;
			 int			 _prevPingModeTime;
			 int			 _prevPingSysTime;
			 int			 _prevMaxTripTime;
			 double			 *_calData;
			 int			  _calDataLen;

	public:
		delegate		void changeCounts(int modeTime, uint16_t bitWord, uint16_t statusWord, uint32_t sysTime, int temp, int sent, int rcvd, int elapsed);
			 changeCounts^	_myDelegate;
	    delegate		void enablePing(int sent, int rcvd);
			 enablePing^	_enablePingDelegate;

	protected: 

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  filleToolStripMenuItem;




	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;






	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  versionToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;



































































































































































private: System::Windows::Forms::ComboBox^  comboBox1;













































































































private: System::Windows::Forms::Button^  OpenCom;
private: System::Windows::Forms::GroupBox^  groupBox20;


















































private: System::Windows::Forms::ToolStripStatusLabel^  errorLabel;






























































































































































































































































































































































































private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;














































































































































































































































private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::ComboBox^  baudRate;




























































































































































































private: System::Windows::Forms::Button^  button12;
private: System::Windows::Forms::TabPage^  tabPage13;
private: System::Windows::Forms::GroupBox^  groupBox17;
private: System::Windows::Forms::TextBox^  data2Tstamp;
private: System::Windows::Forms::Label^  label44;
private: System::Windows::Forms::TextBox^  data2LL;
private: System::Windows::Forms::TextBox^  data2Double;
private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::TextBox^  data2Char;
private: System::Windows::Forms::TextBox^  data2Short;
private: System::Windows::Forms::TextBox^  data2Int;
private: System::Windows::Forms::Label^  label51;
private: System::Windows::Forms::Label^  Short;
private: System::Windows::Forms::Label^  Char;
private: System::Windows::Forms::GroupBox^  groupBox18;
private: System::Windows::Forms::TextBox^  d2Resp;
private: System::Windows::Forms::Button^  button21;
private: System::Windows::Forms::TabPage^  tabPage10;
private: System::Windows::Forms::GroupBox^  groupBox13;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::TextBox^  defaultsResponse;
private: System::Windows::Forms::GroupBox^  groupBox14;
private: System::Windows::Forms::Button^  button13;
private: System::Windows::Forms::TabPage^  tabPage9;
private: System::Windows::Forms::GroupBox^  groupBox11;
private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::TextBox^  saveResponse;
private: System::Windows::Forms::GroupBox^  groupBox12;
private: System::Windows::Forms::Button^  button11;
private: System::Windows::Forms::TabPage^  SetFields;
private: System::Windows::Forms::Button^  button16;
private: System::Windows::Forms::Button^  button15;
private: System::Windows::Forms::GroupBox^  groupBox10;
private: System::Windows::Forms::Button^  button14;
private: System::Windows::Forms::Button^  getRate;
private: System::Windows::Forms::Button^  button10;
private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::TextBox^  unitOrientation;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::TextBox^  unitGyroLpf;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::TextBox^  unitAccelLpf;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::TextBox^  unitPacketType;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::TextBox^  unitBaudRate;
private: System::Windows::Forms::TextBox^  unitPacketRate;
private: System::Windows::Forms::GroupBox^  groupBox9;
private: System::Windows::Forms::Label^  aa;
private: System::Windows::Forms::TextBox^  sparamVal;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TextBox^  sparamNum;
private: System::Windows::Forms::TextBox^  sparamRes;
private: System::Windows::Forms::GroupBox^  groupBox7;
private: System::Windows::Forms::ComboBox^  usrRateLpf;
private: System::Windows::Forms::ComboBox^  usrAccelLpf;
private: System::Windows::Forms::ComboBox^  usrPacketType;
private: System::Windows::Forms::ComboBox^  usrBaudRate;
private: System::Windows::Forms::ComboBox^  usrPacketRate;
private: System::Windows::Forms::GroupBox^  groupBox8;
private: System::Windows::Forms::Button^  button9;
private: System::Windows::Forms::Button^  button8;
private: System::Windows::Forms::Button^  button7;
private: System::Windows::Forms::Button^  button6;
private: System::Windows::Forms::Button^  button5;
private: System::Windows::Forms::Button^  setPacketRate;
private: System::Windows::Forms::TabPage^  tabPage6;
private: System::Windows::Forms::GroupBox^  groupBox5;
private: System::Windows::Forms::Label^  label43;
private: System::Windows::Forms::TextBox^  data1Tstamp;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::TextBox^  data1Zmag;
private: System::Windows::Forms::TextBox^  data1Ymag;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::TextBox^  data1Xmag;
private: System::Windows::Forms::TextBox^  data1Xaccel;
private: System::Windows::Forms::TextBox^  data1Yaccel;
private: System::Windows::Forms::TextBox^  data1Zaccel;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::TextBox^  data1Xrate;
private: System::Windows::Forms::TextBox^  data1Yrate;
private: System::Windows::Forms::TextBox^  data1Zrate;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::GroupBox^  groupBox6;
private: System::Windows::Forms::TextBox^  d1Res;
private: System::Windows::Forms::Button^  button4;
private: System::Windows::Forms::TabPage^  tabPage5;
private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::TextBox^  getUTResponse;
private: System::Windows::Forms::GroupBox^  groupBox4;
private: System::Windows::Forms::Button^  button3;
private: System::Windows::Forms::TabPage^  tabPage7;
private: System::Windows::Forms::GroupBox^  groupBox31;
private: System::Windows::Forms::Label^  label108;
private: System::Windows::Forms::Label^  label107;
private: System::Windows::Forms::TextBox^  S0Zmag;
private: System::Windows::Forms::TextBox^  S0Ymag;
private: System::Windows::Forms::Label^  label106;
private: System::Windows::Forms::TextBox^  S0BoardTemp;
private: System::Windows::Forms::Label^  label105;
private: System::Windows::Forms::TextBox^  S0Temp;
private: System::Windows::Forms::Label^  label104;
private: System::Windows::Forms::TextBox^  S0Xmag;
private: System::Windows::Forms::TextBox^  S0XAccel;
private: System::Windows::Forms::TextBox^  S0YAccel;
private: System::Windows::Forms::TextBox^  S0ZAccel;
private: System::Windows::Forms::Label^  label101;
private: System::Windows::Forms::Label^  label102;
private: System::Windows::Forms::Label^  label103;
private: System::Windows::Forms::Label^  label86;
private: System::Windows::Forms::TextBox^  S0BitStatus;
private: System::Windows::Forms::TextBox^  S0Itow;
private: System::Windows::Forms::TextBox^  S0Xrate;
private: System::Windows::Forms::TextBox^  S0Yrate;
private: System::Windows::Forms::TextBox^  S0Zrate;
private: System::Windows::Forms::Label^  label92;
private: System::Windows::Forms::Label^  label94;
private: System::Windows::Forms::Label^  label99;
private: System::Windows::Forms::Label^  label100;
private: System::Windows::Forms::GroupBox^  groupBox32;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::TabPage^  tabPage4;
private: System::Windows::Forms::GroupBox^  groupBox22;
private: System::Windows::Forms::TextBox^  UsrPingResponse;
private: System::Windows::Forms::GroupBox^  groupBox21;




private: System::Windows::Forms::Button^  UsrPingSend;
private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::GroupBox^  groupBox25;
private: System::Windows::Forms::Label^  label90;
private: System::Windows::Forms::TextBox^  PingResponse;
private: System::Windows::Forms::GroupBox^  groupBox26;
private: System::Windows::Forms::Button^  StartSend;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::GroupBox^  groupBox23;
private: System::Windows::Forms::Label^  label77;
private: System::Windows::Forms::TextBox^  SwResetResponse;
private: System::Windows::Forms::GroupBox^  groupBox24;
private: System::Windows::Forms::Button^  button23;
private: System::Windows::Forms::Button^  SwResetSend;
private: System::Windows::Forms::TabControl^  AnavTab;
private: System::Windows::Forms::Button^  button17;
















































































































































	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->filleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->versionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->errorLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->OpenCom = (gcnew System::Windows::Forms::Button());
			this->groupBox20 = (gcnew System::Windows::Forms::GroupBox());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->baudRate = (gcnew System::Windows::Forms::ComboBox());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabPage13 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox17 = (gcnew System::Windows::Forms::GroupBox());
			this->data2Tstamp = (gcnew System::Windows::Forms::TextBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->data2LL = (gcnew System::Windows::Forms::TextBox());
			this->data2Double = (gcnew System::Windows::Forms::TextBox());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->data2Char = (gcnew System::Windows::Forms::TextBox());
			this->data2Short = (gcnew System::Windows::Forms::TextBox());
			this->data2Int = (gcnew System::Windows::Forms::TextBox());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->Short = (gcnew System::Windows::Forms::Label());
			this->Char = (gcnew System::Windows::Forms::Label());
			this->groupBox18 = (gcnew System::Windows::Forms::GroupBox());
			this->d2Resp = (gcnew System::Windows::Forms::TextBox());
			this->button21 = (gcnew System::Windows::Forms::Button());
			this->tabPage10 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->defaultsResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox14 = (gcnew System::Windows::Forms::GroupBox());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->tabPage9 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->saveResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->SetFields = (gcnew System::Windows::Forms::TabPage());
			this->button16 = (gcnew System::Windows::Forms::Button());
			this->button15 = (gcnew System::Windows::Forms::Button());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->getRate = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->unitOrientation = (gcnew System::Windows::Forms::TextBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->unitGyroLpf = (gcnew System::Windows::Forms::TextBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->unitAccelLpf = (gcnew System::Windows::Forms::TextBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->unitPacketType = (gcnew System::Windows::Forms::TextBox());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->unitBaudRate = (gcnew System::Windows::Forms::TextBox());
			this->unitPacketRate = (gcnew System::Windows::Forms::TextBox());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->aa = (gcnew System::Windows::Forms::Label());
			this->sparamVal = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->sparamNum = (gcnew System::Windows::Forms::TextBox());
			this->sparamRes = (gcnew System::Windows::Forms::TextBox());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->usrRateLpf = (gcnew System::Windows::Forms::ComboBox());
			this->usrAccelLpf = (gcnew System::Windows::Forms::ComboBox());
			this->usrPacketType = (gcnew System::Windows::Forms::ComboBox());
			this->usrBaudRate = (gcnew System::Windows::Forms::ComboBox());
			this->usrPacketRate = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->setPacketRate = (gcnew System::Windows::Forms::Button());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->data1Tstamp = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->data1Zmag = (gcnew System::Windows::Forms::TextBox());
			this->data1Ymag = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->data1Xmag = (gcnew System::Windows::Forms::TextBox());
			this->data1Xaccel = (gcnew System::Windows::Forms::TextBox());
			this->data1Yaccel = (gcnew System::Windows::Forms::TextBox());
			this->data1Zaccel = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->data1Xrate = (gcnew System::Windows::Forms::TextBox());
			this->data1Yrate = (gcnew System::Windows::Forms::TextBox());
			this->data1Zrate = (gcnew System::Windows::Forms::TextBox());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->d1Res = (gcnew System::Windows::Forms::TextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->getUTResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox31 = (gcnew System::Windows::Forms::GroupBox());
			this->label108 = (gcnew System::Windows::Forms::Label());
			this->label107 = (gcnew System::Windows::Forms::Label());
			this->S0Zmag = (gcnew System::Windows::Forms::TextBox());
			this->S0Ymag = (gcnew System::Windows::Forms::TextBox());
			this->label106 = (gcnew System::Windows::Forms::Label());
			this->S0BoardTemp = (gcnew System::Windows::Forms::TextBox());
			this->label105 = (gcnew System::Windows::Forms::Label());
			this->S0Temp = (gcnew System::Windows::Forms::TextBox());
			this->label104 = (gcnew System::Windows::Forms::Label());
			this->S0Xmag = (gcnew System::Windows::Forms::TextBox());
			this->S0XAccel = (gcnew System::Windows::Forms::TextBox());
			this->S0YAccel = (gcnew System::Windows::Forms::TextBox());
			this->S0ZAccel = (gcnew System::Windows::Forms::TextBox());
			this->label101 = (gcnew System::Windows::Forms::Label());
			this->label102 = (gcnew System::Windows::Forms::Label());
			this->label103 = (gcnew System::Windows::Forms::Label());
			this->label86 = (gcnew System::Windows::Forms::Label());
			this->S0BitStatus = (gcnew System::Windows::Forms::TextBox());
			this->S0Itow = (gcnew System::Windows::Forms::TextBox());
			this->S0Xrate = (gcnew System::Windows::Forms::TextBox());
			this->S0Yrate = (gcnew System::Windows::Forms::TextBox());
			this->S0Zrate = (gcnew System::Windows::Forms::TextBox());
			this->label92 = (gcnew System::Windows::Forms::Label());
			this->label94 = (gcnew System::Windows::Forms::Label());
			this->label99 = (gcnew System::Windows::Forms::Label());
			this->label100 = (gcnew System::Windows::Forms::Label());
			this->groupBox32 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox22 = (gcnew System::Windows::Forms::GroupBox());
			this->UsrPingResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox21 = (gcnew System::Windows::Forms::GroupBox());
			this->button17 = (gcnew System::Windows::Forms::Button());
			this->UsrPingSend = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox25 = (gcnew System::Windows::Forms::GroupBox());
			this->label90 = (gcnew System::Windows::Forms::Label());
			this->PingResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox26 = (gcnew System::Windows::Forms::GroupBox());
			this->StartSend = (gcnew System::Windows::Forms::Button());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox23 = (gcnew System::Windows::Forms::GroupBox());
			this->label77 = (gcnew System::Windows::Forms::Label());
			this->SwResetResponse = (gcnew System::Windows::Forms::TextBox());
			this->groupBox24 = (gcnew System::Windows::Forms::GroupBox());
			this->button23 = (gcnew System::Windows::Forms::Button());
			this->SwResetSend = (gcnew System::Windows::Forms::Button());
			this->AnavTab = (gcnew System::Windows::Forms::TabControl());
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->groupBox20->SuspendLayout();
			this->tabPage13->SuspendLayout();
			this->groupBox17->SuspendLayout();
			this->groupBox18->SuspendLayout();
			this->tabPage10->SuspendLayout();
			this->groupBox13->SuspendLayout();
			this->groupBox14->SuspendLayout();
			this->tabPage9->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->SetFields->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->tabPage6->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->tabPage7->SuspendLayout();
			this->groupBox31->SuspendLayout();
			this->groupBox32->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->groupBox22->SuspendLayout();
			this->groupBox21->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox25->SuspendLayout();
			this->groupBox26->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox23->SuspendLayout();
			this->groupBox24->SuspendLayout();
			this->AnavTab->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->filleToolStripMenuItem,
					this->helpToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(810, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// filleToolStripMenuItem
			// 
			this->filleToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->exitToolStripMenuItem2 });
			this->filleToolStripMenuItem->Name = L"filleToolStripMenuItem";
			this->filleToolStripMenuItem->Size = System::Drawing::Size(43, 20);
			this->filleToolStripMenuItem->Text = L"Fille";
			// 
			// exitToolStripMenuItem2
			// 
			this->exitToolStripMenuItem2->Name = L"exitToolStripMenuItem2";
			this->exitToolStripMenuItem2->Size = System::Drawing::Size(96, 22);
			this->exitToolStripMenuItem2->Text = L"Exit";
			this->exitToolStripMenuItem2->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem2_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(45, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->versionToolStripMenuItem });
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(109, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			// 
			// versionToolStripMenuItem
			// 
			this->versionToolStripMenuItem->Name = L"versionToolStripMenuItem";
			this->versionToolStripMenuItem->Size = System::Drawing::Size(119, 22);
			this->versionToolStripMenuItem->Text = L"Version";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripStatusLabel1,
					this->errorLabel
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 540);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(810, 22);
			this->statusStrip1->TabIndex = 6;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(0, 17);
			// 
			// errorLabel
			// 
			this->errorLabel->ForeColor = System::Drawing::Color::Red;
			this->errorLabel->Name = L"errorLabel";
			this->errorLabel->Size = System::Drawing::Size(49, 20);
			this->errorLabel->Text = L"Error";
			this->errorLabel->Visible = false;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->DefaultExt = L"csv";
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"csv files (*.csv)|*.csv";
			this->openFileDialog1->InitialDirectory = L".";
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->DefaultExt = L"csv";
			this->saveFileDialog1->FileName = L"ProbeData";
			this->saveFileDialog1->Filter = L"csv files (*.csv)|*.csv";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(9, 25);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 19;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// OpenCom
			// 
			this->OpenCom->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->OpenCom->Location = System::Drawing::Point(145, 21);
			this->OpenCom->Name = L"OpenCom";
			this->OpenCom->Size = System::Drawing::Size(100, 25);
			this->OpenCom->TabIndex = 20;
			this->OpenCom->Text = L"Open";
			this->OpenCom->UseVisualStyleBackColor = true;
			this->OpenCom->Click += gcnew System::EventHandler(this, &Form1::OpenCom_Click);
			// 
			// groupBox20
			// 
			this->groupBox20->Controls->Add(this->button12);
			this->groupBox20->Controls->Add(this->label1);
			this->groupBox20->Controls->Add(this->baudRate);
			this->groupBox20->Controls->Add(this->OpenCom);
			this->groupBox20->Controls->Add(this->comboBox1);
			this->groupBox20->Location = System::Drawing::Point(16, 27);
			this->groupBox20->Name = L"groupBox20";
			this->groupBox20->Size = System::Drawing::Size(267, 109);
			this->groupBox20->TabIndex = 21;
			this->groupBox20->TabStop = false;
			this->groupBox20->Text = L"COM Port";
			// 
			// button12
			// 
			this->button12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button12->Location = System::Drawing::Point(145, 67);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(100, 25);
			this->button12->TabIndex = 31;
			this->button12->Text = L"Close";
			this->button12->UseVisualStyleBackColor = true;
			this->button12->Click += gcnew System::EventHandler(this, &Form1::button12_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 55);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 13);
			this->label1->TabIndex = 30;
			this->label1->Text = L"BaudRate";
			// 
			// baudRate
			// 
			this->baudRate->FormattingEnabled = true;
			this->baudRate->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"38400", L"57600", L"115200", L"230400", L"460800" });
			this->baudRate->Location = System::Drawing::Point(9, 71);
			this->baudRate->Name = L"baudRate";
			this->baudRate->Size = System::Drawing::Size(121, 21);
			this->baudRate->TabIndex = 21;
			this->baudRate->Text = L"115200";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// tabPage13
			// 
			this->tabPage13->Controls->Add(this->groupBox17);
			this->tabPage13->Controls->Add(this->groupBox18);
			this->tabPage13->Location = System::Drawing::Point(4, 22);
			this->tabPage13->Name = L"tabPage13";
			this->tabPage13->Padding = System::Windows::Forms::Padding(3);
			this->tabPage13->Size = System::Drawing::Size(767, 316);
			this->tabPage13->TabIndex = 22;
			this->tabPage13->Text = L"Get Data2";
			this->tabPage13->UseVisualStyleBackColor = true;
			// 
			// groupBox17
			// 
			this->groupBox17->Controls->Add(this->data2Tstamp);
			this->groupBox17->Controls->Add(this->label44);
			this->groupBox17->Controls->Add(this->data2LL);
			this->groupBox17->Controls->Add(this->data2Double);
			this->groupBox17->Controls->Add(this->label49);
			this->groupBox17->Controls->Add(this->label50);
			this->groupBox17->Controls->Add(this->data2Char);
			this->groupBox17->Controls->Add(this->data2Short);
			this->groupBox17->Controls->Add(this->data2Int);
			this->groupBox17->Controls->Add(this->label51);
			this->groupBox17->Controls->Add(this->Short);
			this->groupBox17->Controls->Add(this->Char);
			this->groupBox17->Location = System::Drawing::Point(182, 19);
			this->groupBox17->Name = L"groupBox17";
			this->groupBox17->Size = System::Drawing::Size(427, 279);
			this->groupBox17->TabIndex = 29;
			this->groupBox17->TabStop = false;
			this->groupBox17->Text = L"Response";
			// 
			// data2Tstamp
			// 
			this->data2Tstamp->Location = System::Drawing::Point(21, 210);
			this->data2Tstamp->Name = L"data2Tstamp";
			this->data2Tstamp->Size = System::Drawing::Size(126, 20);
			this->data2Tstamp->TabIndex = 41;
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(154, 213);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(42, 13);
			this->label44->TabIndex = 40;
			this->label44->Text = L"Tstamp";
			// 
			// data2LL
			// 
			this->data2LL->Location = System::Drawing::Point(20, 137);
			this->data2LL->Name = L"data2LL";
			this->data2LL->Size = System::Drawing::Size(127, 20);
			this->data2LL->TabIndex = 23;
			// 
			// data2Double
			// 
			this->data2Double->Location = System::Drawing::Point(20, 169);
			this->data2Double->Name = L"data2Double";
			this->data2Double->Size = System::Drawing::Size(127, 20);
			this->data2Double->TabIndex = 24;
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(154, 172);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(41, 13);
			this->label49->TabIndex = 26;
			this->label49->Text = L"Double";
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(154, 140);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(55, 13);
			this->label50->TabIndex = 27;
			this->label50->Text = L"LongLong";
			// 
			// data2Char
			// 
			this->data2Char->Location = System::Drawing::Point(20, 24);
			this->data2Char->Name = L"data2Char";
			this->data2Char->Size = System::Drawing::Size(127, 20);
			this->data2Char->TabIndex = 2;
			// 
			// data2Short
			// 
			this->data2Short->Location = System::Drawing::Point(20, 58);
			this->data2Short->Name = L"data2Short";
			this->data2Short->Size = System::Drawing::Size(127, 20);
			this->data2Short->TabIndex = 3;
			// 
			// data2Int
			// 
			this->data2Int->Location = System::Drawing::Point(21, 94);
			this->data2Int->Name = L"data2Int";
			this->data2Int->Size = System::Drawing::Size(126, 20);
			this->data2Int->TabIndex = 4;
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(154, 94);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(19, 13);
			this->label51->TabIndex = 11;
			this->label51->Text = L"Int";
			// 
			// Short
			// 
			this->Short->AutoSize = true;
			this->Short->Location = System::Drawing::Point(154, 61);
			this->Short->Name = L"Short";
			this->Short->Size = System::Drawing::Size(32, 13);
			this->Short->TabIndex = 7;
			this->Short->Text = L"Short";
			// 
			// Char
			// 
			this->Char->AutoSize = true;
			this->Char->Location = System::Drawing::Point(154, 27);
			this->Char->Name = L"Char";
			this->Char->Size = System::Drawing::Size(29, 13);
			this->Char->TabIndex = 8;
			this->Char->Text = L"Char";
			// 
			// groupBox18
			// 
			this->groupBox18->Controls->Add(this->d2Resp);
			this->groupBox18->Controls->Add(this->button21);
			this->groupBox18->Location = System::Drawing::Point(27, 21);
			this->groupBox18->Name = L"groupBox18";
			this->groupBox18->Size = System::Drawing::Size(138, 277);
			this->groupBox18->TabIndex = 28;
			this->groupBox18->TabStop = false;
			this->groupBox18->Text = L"Command";
			// 
			// d2Resp
			// 
			this->d2Resp->Location = System::Drawing::Point(10, 80);
			this->d2Resp->Name = L"d2Resp";
			this->d2Resp->Size = System::Drawing::Size(100, 20);
			this->d2Resp->TabIndex = 17;
			// 
			// button21
			// 
			this->button21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button21->Location = System::Drawing::Point(10, 29);
			this->button21->Name = L"button21";
			this->button21->Size = System::Drawing::Size(100, 25);
			this->button21->TabIndex = 16;
			this->button21->Text = L"Send";
			this->button21->UseVisualStyleBackColor = true;
			this->button21->Click += gcnew System::EventHandler(this, &Form1::button21_Click);
			// 
			// tabPage10
			// 
			this->tabPage10->Controls->Add(this->groupBox13);
			this->tabPage10->Controls->Add(this->groupBox14);
			this->tabPage10->Location = System::Drawing::Point(4, 22);
			this->tabPage10->Name = L"tabPage10";
			this->tabPage10->Padding = System::Windows::Forms::Padding(3);
			this->tabPage10->Size = System::Drawing::Size(767, 316);
			this->tabPage10->TabIndex = 19;
			this->tabPage10->Text = L"Reset To Defaults";
			this->tabPage10->UseVisualStyleBackColor = true;
			// 
			// groupBox13
			// 
			this->groupBox13->Controls->Add(this->label41);
			this->groupBox13->Controls->Add(this->defaultsResponse);
			this->groupBox13->Location = System::Drawing::Point(185, 17);
			this->groupBox13->Name = L"groupBox13";
			this->groupBox13->Size = System::Drawing::Size(195, 281);
			this->groupBox13->TabIndex = 26;
			this->groupBox13->TabStop = false;
			this->groupBox13->Text = L"Response";
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(105, 31);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(37, 13);
			this->label41->TabIndex = 11;
			this->label41->Text = L"Status";
			// 
			// defaultsResponse
			// 
			this->defaultsResponse->Location = System::Drawing::Point(12, 24);
			this->defaultsResponse->Name = L"defaultsResponse";
			this->defaultsResponse->Size = System::Drawing::Size(84, 20);
			this->defaultsResponse->TabIndex = 6;
			// 
			// groupBox14
			// 
			this->groupBox14->Controls->Add(this->button13);
			this->groupBox14->Location = System::Drawing::Point(18, 17);
			this->groupBox14->Name = L"groupBox14";
			this->groupBox14->Size = System::Drawing::Size(139, 281);
			this->groupBox14->TabIndex = 25;
			this->groupBox14->TabStop = false;
			this->groupBox14->Text = L"Command";
			// 
			// button13
			// 
			this->button13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button13->Location = System::Drawing::Point(10, 29);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(100, 25);
			this->button13->TabIndex = 16;
			this->button13->Text = L"Send";
			this->button13->UseVisualStyleBackColor = true;
			this->button13->Click += gcnew System::EventHandler(this, &Form1::button13_Click);
			// 
			// tabPage9
			// 
			this->tabPage9->Controls->Add(this->groupBox11);
			this->tabPage9->Controls->Add(this->groupBox12);
			this->tabPage9->Location = System::Drawing::Point(4, 22);
			this->tabPage9->Name = L"tabPage9";
			this->tabPage9->Padding = System::Windows::Forms::Padding(3);
			this->tabPage9->Size = System::Drawing::Size(767, 316);
			this->tabPage9->TabIndex = 18;
			this->tabPage9->Text = L"Save Params";
			this->tabPage9->UseVisualStyleBackColor = true;
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->label40);
			this->groupBox11->Controls->Add(this->saveResponse);
			this->groupBox11->Location = System::Drawing::Point(188, 20);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(195, 281);
			this->groupBox11->TabIndex = 24;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"Response";
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(105, 31);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(37, 13);
			this->label40->TabIndex = 11;
			this->label40->Text = L"Status";
			// 
			// saveResponse
			// 
			this->saveResponse->Location = System::Drawing::Point(12, 24);
			this->saveResponse->Name = L"saveResponse";
			this->saveResponse->Size = System::Drawing::Size(84, 20);
			this->saveResponse->TabIndex = 6;
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->button11);
			this->groupBox12->Location = System::Drawing::Point(21, 20);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(139, 281);
			this->groupBox12->TabIndex = 23;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"Command";
			// 
			// button11
			// 
			this->button11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button11->Location = System::Drawing::Point(10, 29);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(100, 25);
			this->button11->TabIndex = 16;
			this->button11->Text = L"Send";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &Form1::button11_Click);
			// 
			// SetFields
			// 
			this->SetFields->Controls->Add(this->button16);
			this->SetFields->Controls->Add(this->button15);
			this->SetFields->Controls->Add(this->groupBox10);
			this->SetFields->Controls->Add(this->groupBox9);
			this->SetFields->Controls->Add(this->groupBox7);
			this->SetFields->Controls->Add(this->groupBox8);
			this->SetFields->Location = System::Drawing::Point(4, 22);
			this->SetFields->Name = L"SetFields";
			this->SetFields->Padding = System::Windows::Forms::Padding(3);
			this->SetFields->Size = System::Drawing::Size(767, 316);
			this->SetFields->TabIndex = 17;
			this->SetFields->Text = L"Set Params";
			this->SetFields->UseVisualStyleBackColor = true;
			this->SetFields->Enter += gcnew System::EventHandler(this, &Form1::SetFields_Enter);
			// 
			// button16
			// 
			this->button16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button16->Location = System::Drawing::Point(291, 175);
			this->button16->Name = L"button16";
			this->button16->Size = System::Drawing::Size(122, 25);
			this->button16->TabIndex = 27;
			this->button16->Text = L"Set All";
			this->button16->UseVisualStyleBackColor = true;
			this->button16->Click += gcnew System::EventHandler(this, &Form1::button16_Click);
			// 
			// button15
			// 
			this->button15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button15->Location = System::Drawing::Point(291, 136);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(122, 25);
			this->button15->TabIndex = 26;
			this->button15->Text = L"Set Config";
			this->button15->UseVisualStyleBackColor = true;
			this->button15->Click += gcnew System::EventHandler(this, &Form1::button15_Click);
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->button14);
			this->groupBox10->Controls->Add(this->getRate);
			this->groupBox10->Controls->Add(this->button10);
			this->groupBox10->Controls->Add(this->label39);
			this->groupBox10->Controls->Add(this->unitOrientation);
			this->groupBox10->Controls->Add(this->label38);
			this->groupBox10->Controls->Add(this->unitGyroLpf);
			this->groupBox10->Controls->Add(this->label37);
			this->groupBox10->Controls->Add(this->unitAccelLpf);
			this->groupBox10->Controls->Add(this->label31);
			this->groupBox10->Controls->Add(this->unitPacketType);
			this->groupBox10->Controls->Add(this->label33);
			this->groupBox10->Controls->Add(this->label36);
			this->groupBox10->Controls->Add(this->unitBaudRate);
			this->groupBox10->Controls->Add(this->unitPacketRate);
			this->groupBox10->Location = System::Drawing::Point(476, 20);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(280, 279);
			this->groupBox10->TabIndex = 25;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Get Fields";
			// 
			// button14
			// 
			this->button14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button14->Location = System::Drawing::Point(153, 229);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(100, 25);
			this->button14->TabIndex = 24;
			this->button14->Text = L"Get All";
			this->button14->UseVisualStyleBackColor = true;
			this->button14->Click += gcnew System::EventHandler(this, &Form1::button14_Click);
			// 
			// getRate
			// 
			this->getRate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->getRate->Location = System::Drawing::Point(6, 24);
			this->getRate->Name = L"getRate";
			this->getRate->Size = System::Drawing::Size(99, 26);
			this->getRate->TabIndex = 23;
			this->getRate->Text = L"Get Pkt Rate";
			this->getRate->UseVisualStyleBackColor = true;
			this->getRate->Click += gcnew System::EventHandler(this, &Form1::getRate_Click);
			// 
			// button10
			// 
			this->button10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button10->Location = System::Drawing::Point(18, 229);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(100, 25);
			this->button10->TabIndex = 22;
			this->button10->Text = L"Get Config";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::button10_Click);
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(206, 184);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(58, 13);
			this->label39->TabIndex = 11;
			this->label39->Text = L"Orientation";
			// 
			// unitOrientation
			// 
			this->unitOrientation->Location = System::Drawing::Point(111, 182);
			this->unitOrientation->Name = L"unitOrientation";
			this->unitOrientation->Size = System::Drawing::Size(86, 20);
			this->unitOrientation->TabIndex = 10;
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(206, 149);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(47, 13);
			this->label38->TabIndex = 9;
			this->label38->Text = L"Gyro Lpf";
			// 
			// unitGyroLpf
			// 
			this->unitGyroLpf->Location = System::Drawing::Point(110, 149);
			this->unitGyroLpf->Name = L"unitGyroLpf";
			this->unitGyroLpf->Size = System::Drawing::Size(87, 20);
			this->unitGyroLpf->TabIndex = 8;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(203, 115);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(52, 13);
			this->label37->TabIndex = 7;
			this->label37->Text = L"Accel Lpf";
			// 
			// unitAccelLpf
			// 
			this->unitAccelLpf->Location = System::Drawing::Point(111, 116);
			this->unitAccelLpf->Name = L"unitAccelLpf";
			this->unitAccelLpf->Size = System::Drawing::Size(86, 20);
			this->unitAccelLpf->TabIndex = 6;
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(206, 89);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(68, 13);
			this->label31->TabIndex = 5;
			this->label31->Text = L"Packet Type";
			// 
			// unitPacketType
			// 
			this->unitPacketType->Location = System::Drawing::Point(111, 86);
			this->unitPacketType->Name = L"unitPacketType";
			this->unitPacketType->Size = System::Drawing::Size(86, 20);
			this->unitPacketType->TabIndex = 4;
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(203, 59);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(58, 13);
			this->label33->TabIndex = 3;
			this->label33->Text = L"Baud Rate";
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(203, 27);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(67, 13);
			this->label36->TabIndex = 2;
			this->label36->Text = L"Packet Rate";
			// 
			// unitBaudRate
			// 
			this->unitBaudRate->Location = System::Drawing::Point(111, 56);
			this->unitBaudRate->Name = L"unitBaudRate";
			this->unitBaudRate->Size = System::Drawing::Size(86, 20);
			this->unitBaudRate->TabIndex = 1;
			// 
			// unitPacketRate
			// 
			this->unitPacketRate->Location = System::Drawing::Point(111, 24);
			this->unitPacketRate->Name = L"unitPacketRate";
			this->unitPacketRate->Size = System::Drawing::Size(86, 20);
			this->unitPacketRate->TabIndex = 0;
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->aa);
			this->groupBox9->Controls->Add(this->sparamVal);
			this->groupBox9->Controls->Add(this->label6);
			this->groupBox9->Controls->Add(this->label5);
			this->groupBox9->Controls->Add(this->sparamNum);
			this->groupBox9->Controls->Add(this->sparamRes);
			this->groupBox9->Location = System::Drawing::Point(291, 20);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(168, 98);
			this->groupBox9->TabIndex = 24;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Response";
			// 
			// aa
			// 
			this->aa->AutoSize = true;
			this->aa->Location = System::Drawing::Point(18, 69);
			this->aa->Name = L"aa";
			this->aa->Size = System::Drawing::Size(34, 13);
			this->aa->TabIndex = 5;
			this->aa->Text = L"Value";
			// 
			// sparamVal
			// 
			this->sparamVal->Location = System::Drawing::Point(86, 69);
			this->sparamVal->Name = L"sparamVal";
			this->sparamVal->Size = System::Drawing::Size(72, 20);
			this->sparamVal->TabIndex = 4;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(15, 43);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(67, 13);
			this->label6->TabIndex = 3;
			this->label6->Text = L"Num Params";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(15, 16);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(37, 13);
			this->label5->TabIndex = 2;
			this->label5->Text = L"Result";
			// 
			// sparamNum
			// 
			this->sparamNum->Location = System::Drawing::Point(86, 43);
			this->sparamNum->Name = L"sparamNum";
			this->sparamNum->Size = System::Drawing::Size(72, 20);
			this->sparamNum->TabIndex = 1;
			// 
			// sparamRes
			// 
			this->sparamRes->Location = System::Drawing::Point(86, 16);
			this->sparamRes->Name = L"sparamRes";
			this->sparamRes->Size = System::Drawing::Size(72, 20);
			this->sparamRes->TabIndex = 0;
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->usrRateLpf);
			this->groupBox7->Controls->Add(this->usrAccelLpf);
			this->groupBox7->Controls->Add(this->usrPacketType);
			this->groupBox7->Controls->Add(this->usrBaudRate);
			this->groupBox7->Controls->Add(this->usrPacketRate);
			this->groupBox7->Location = System::Drawing::Point(172, 18);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(104, 279);
			this->groupBox7->TabIndex = 23;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Parameter";
			// 
			// usrRateLpf
			// 
			this->usrRateLpf->FormattingEnabled = true;
			this->usrRateLpf->Items->AddRange(gcnew cli::array< System::Object^  >(8) {
				L"0", L"2", L"5", L"10", L"20", L"25", L"40",
					L"50"
			});
			this->usrRateLpf->Location = System::Drawing::Point(15, 197);
			this->usrRateLpf->Name = L"usrRateLpf";
			this->usrRateLpf->Size = System::Drawing::Size(67, 21);
			this->usrRateLpf->TabIndex = 4;
			this->usrRateLpf->Text = L"0";
			// 
			// usrAccelLpf
			// 
			this->usrAccelLpf->FormattingEnabled = true;
			this->usrAccelLpf->Items->AddRange(gcnew cli::array< System::Object^  >(8) {
				L"0", L"2", L"5", L"10", L"20", L"25", L"40",
					L"50"
			});
			this->usrAccelLpf->Location = System::Drawing::Point(15, 157);
			this->usrAccelLpf->Name = L"usrAccelLpf";
			this->usrAccelLpf->Size = System::Drawing::Size(67, 21);
			this->usrAccelLpf->TabIndex = 3;
			this->usrAccelLpf->Text = L"0";
			// 
			// usrPacketType
			// 
			this->usrPacketType->FormattingEnabled = true;
			this->usrPacketType->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"zT", L"z1", L"z2" });
			this->usrPacketType->Location = System::Drawing::Point(15, 116);
			this->usrPacketType->Name = L"usrPacketType";
			this->usrPacketType->Size = System::Drawing::Size(67, 21);
			this->usrPacketType->TabIndex = 2;
			this->usrPacketType->Text = L"zT";
			// 
			// usrBaudRate
			// 
			this->usrBaudRate->FormattingEnabled = true;
			this->usrBaudRate->Items->AddRange(gcnew cli::array< System::Object^  >(8) {
				L"4800", L"9600", L"19200", L"38400", L"57600",
					L"115200", L"230400", L"460800"
			});
			this->usrBaudRate->Location = System::Drawing::Point(15, 76);
			this->usrBaudRate->Name = L"usrBaudRate";
			this->usrBaudRate->Size = System::Drawing::Size(67, 21);
			this->usrBaudRate->TabIndex = 1;
			this->usrBaudRate->Text = L"57600";
			// 
			// usrPacketRate
			// 
			this->usrPacketRate->FormattingEnabled = true;
			this->usrPacketRate->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"0", L"2", L"5", L"10", L"20", L"25", L"50",
					L"100", L"200"
			});
			this->usrPacketRate->Location = System::Drawing::Point(15, 31);
			this->usrPacketRate->Name = L"usrPacketRate";
			this->usrPacketRate->Size = System::Drawing::Size(67, 21);
			this->usrPacketRate->TabIndex = 0;
			this->usrPacketRate->Text = L"0";
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->button9);
			this->groupBox8->Controls->Add(this->button8);
			this->groupBox8->Controls->Add(this->button7);
			this->groupBox8->Controls->Add(this->button6);
			this->groupBox8->Controls->Add(this->button5);
			this->groupBox8->Controls->Add(this->setPacketRate);
			this->groupBox8->Location = System::Drawing::Point(17, 20);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(138, 277);
			this->groupBox8->TabIndex = 22;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Command";
			// 
			// button9
			// 
			this->button9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button9->Location = System::Drawing::Point(10, 230);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(122, 25);
			this->button9->TabIndex = 21;
			this->button9->Text = L"Set Orientation";
			this->button9->UseVisualStyleBackColor = true;
			// 
			// button8
			// 
			this->button8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button8->Location = System::Drawing::Point(10, 192);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(122, 25);
			this->button8->TabIndex = 20;
			this->button8->Text = L"Set Gyro LPF";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click);
			// 
			// button7
			// 
			this->button7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button7->Location = System::Drawing::Point(10, 152);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(122, 25);
			this->button7->TabIndex = 19;
			this->button7->Text = L"Set Accel LPF";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button6
			// 
			this->button6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button6->Location = System::Drawing::Point(10, 111);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(122, 25);
			this->button6->TabIndex = 18;
			this->button6->Text = L"Set Packet Type";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button5->Location = System::Drawing::Point(10, 71);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(122, 25);
			this->button5->TabIndex = 17;
			this->button5->Text = L"Set Baud Rate";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// setPacketRate
			// 
			this->setPacketRate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->setPacketRate->Location = System::Drawing::Point(10, 29);
			this->setPacketRate->Name = L"setPacketRate";
			this->setPacketRate->Size = System::Drawing::Size(122, 25);
			this->setPacketRate->TabIndex = 16;
			this->setPacketRate->Text = L"Set Packet Rate";
			this->setPacketRate->UseVisualStyleBackColor = true;
			this->setPacketRate->Click += gcnew System::EventHandler(this, &Form1::setPacketRate_Click);
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->groupBox5);
			this->tabPage6->Controls->Add(this->groupBox6);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Padding = System::Windows::Forms::Padding(3);
			this->tabPage6->Size = System::Drawing::Size(767, 316);
			this->tabPage6->TabIndex = 16;
			this->tabPage6->Text = L"Get Data1";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->label43);
			this->groupBox5->Controls->Add(this->data1Tstamp);
			this->groupBox5->Controls->Add(this->label3);
			this->groupBox5->Controls->Add(this->label4);
			this->groupBox5->Controls->Add(this->data1Zmag);
			this->groupBox5->Controls->Add(this->data1Ymag);
			this->groupBox5->Controls->Add(this->label7);
			this->groupBox5->Controls->Add(this->data1Xmag);
			this->groupBox5->Controls->Add(this->data1Xaccel);
			this->groupBox5->Controls->Add(this->data1Yaccel);
			this->groupBox5->Controls->Add(this->data1Zaccel);
			this->groupBox5->Controls->Add(this->label8);
			this->groupBox5->Controls->Add(this->label29);
			this->groupBox5->Controls->Add(this->label30);
			this->groupBox5->Controls->Add(this->data1Xrate);
			this->groupBox5->Controls->Add(this->data1Yrate);
			this->groupBox5->Controls->Add(this->data1Zrate);
			this->groupBox5->Controls->Add(this->label32);
			this->groupBox5->Controls->Add(this->label34);
			this->groupBox5->Controls->Add(this->label35);
			this->groupBox5->Location = System::Drawing::Point(177, 18);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(427, 279);
			this->groupBox5->TabIndex = 27;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Response";
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Location = System::Drawing::Point(321, 140);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(42, 13);
			this->label43->TabIndex = 40;
			this->label43->Text = L"Tstamp";
			// 
			// data1Tstamp
			// 
			this->data1Tstamp->Location = System::Drawing::Point(210, 137);
			this->data1Tstamp->Name = L"data1Tstamp";
			this->data1Tstamp->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->data1Tstamp->Size = System::Drawing::Size(100, 20);
			this->data1Tstamp->TabIndex = 39;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(321, 94);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(34, 13);
			this->label3->TabIndex = 38;
			this->label3->Text = L"Zmag";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(321, 62);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(34, 13);
			this->label4->TabIndex = 37;
			this->label4->Text = L"Ymag";
			// 
			// data1Zmag
			// 
			this->data1Zmag->Location = System::Drawing::Point(210, 90);
			this->data1Zmag->Name = L"data1Zmag";
			this->data1Zmag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->data1Zmag->Size = System::Drawing::Size(100, 20);
			this->data1Zmag->TabIndex = 36;
			// 
			// data1Ymag
			// 
			this->data1Ymag->Location = System::Drawing::Point(210, 55);
			this->data1Ymag->Name = L"data1Ymag";
			this->data1Ymag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->data1Ymag->Size = System::Drawing::Size(100, 20);
			this->data1Ymag->TabIndex = 35;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(321, 27);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(34, 13);
			this->label7->TabIndex = 30;
			this->label7->Text = L"Xmag";
			// 
			// data1Xmag
			// 
			this->data1Xmag->Location = System::Drawing::Point(210, 19);
			this->data1Xmag->Name = L"data1Xmag";
			this->data1Xmag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->data1Xmag->Size = System::Drawing::Size(100, 20);
			this->data1Xmag->TabIndex = 29;
			// 
			// data1Xaccel
			// 
			this->data1Xaccel->Location = System::Drawing::Point(20, 137);
			this->data1Xaccel->Name = L"data1Xaccel";
			this->data1Xaccel->Size = System::Drawing::Size(100, 20);
			this->data1Xaccel->TabIndex = 23;
			// 
			// data1Yaccel
			// 
			this->data1Yaccel->Location = System::Drawing::Point(20, 169);
			this->data1Yaccel->Name = L"data1Yaccel";
			this->data1Yaccel->Size = System::Drawing::Size(100, 20);
			this->data1Yaccel->TabIndex = 24;
			// 
			// data1Zaccel
			// 
			this->data1Zaccel->Location = System::Drawing::Point(20, 200);
			this->data1Zaccel->Name = L"data1Zaccel";
			this->data1Zaccel->Size = System::Drawing::Size(100, 20);
			this->data1Zaccel->TabIndex = 25;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(127, 203);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(41, 13);
			this->label8->TabIndex = 28;
			this->label8->Text = L"ZAccel";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(128, 174);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(41, 13);
			this->label29->TabIndex = 26;
			this->label29->Text = L"YAccel";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(128, 147);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(41, 13);
			this->label30->TabIndex = 27;
			this->label30->Text = L"XAccel";
			// 
			// data1Xrate
			// 
			this->data1Xrate->Location = System::Drawing::Point(20, 24);
			this->data1Xrate->Name = L"data1Xrate";
			this->data1Xrate->Size = System::Drawing::Size(100, 20);
			this->data1Xrate->TabIndex = 2;
			// 
			// data1Yrate
			// 
			this->data1Yrate->Location = System::Drawing::Point(20, 58);
			this->data1Yrate->Name = L"data1Yrate";
			this->data1Yrate->Size = System::Drawing::Size(100, 20);
			this->data1Yrate->TabIndex = 3;
			// 
			// data1Zrate
			// 
			this->data1Zrate->Location = System::Drawing::Point(21, 94);
			this->data1Zrate->Name = L"data1Zrate";
			this->data1Zrate->Size = System::Drawing::Size(100, 20);
			this->data1Zrate->TabIndex = 4;
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(128, 97);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(37, 13);
			this->label32->TabIndex = 11;
			this->label32->Text = L"ZRate";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(128, 62);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(32, 13);
			this->label34->TabIndex = 7;
			this->label34->Text = L"Yrate";
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(127, 28);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(37, 13);
			this->label35->TabIndex = 8;
			this->label35->Text = L"XRate";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->d1Res);
			this->groupBox6->Controls->Add(this->button4);
			this->groupBox6->Location = System::Drawing::Point(22, 20);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(138, 277);
			this->groupBox6->TabIndex = 26;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Command";
			// 
			// d1Res
			// 
			this->d1Res->Location = System::Drawing::Point(10, 80);
			this->d1Res->Name = L"d1Res";
			this->d1Res->Size = System::Drawing::Size(100, 20);
			this->d1Res->TabIndex = 17;
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(10, 29);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(100, 25);
			this->button4->TabIndex = 16;
			this->button4->Text = L"Send";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->groupBox3);
			this->tabPage5->Controls->Add(this->groupBox4);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(767, 316);
			this->tabPage5->TabIndex = 15;
			this->tabPage5->Text = L"Get User Test";
			this->tabPage5->UseVisualStyleBackColor = true;
			this->tabPage5->Enter += gcnew System::EventHandler(this, &Form1::tabPage5_Enter);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label2);
			this->groupBox3->Controls->Add(this->getUTResponse);
			this->groupBox3->Location = System::Drawing::Point(189, 16);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(195, 281);
			this->groupBox3->TabIndex = 24;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Response";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(105, 31);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(37, 13);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Status";
			// 
			// getUTResponse
			// 
			this->getUTResponse->Location = System::Drawing::Point(12, 24);
			this->getUTResponse->Name = L"getUTResponse";
			this->getUTResponse->Size = System::Drawing::Size(84, 20);
			this->getUTResponse->TabIndex = 6;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->button3);
			this->groupBox4->Location = System::Drawing::Point(22, 16);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(139, 281);
			this->groupBox4->TabIndex = 23;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Command";
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(10, 29);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 25);
			this->button3->TabIndex = 16;
			this->button3->Text = L"Send";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click_1);
			// 
			// tabPage7
			// 
			this->tabPage7->Controls->Add(this->groupBox31);
			this->tabPage7->Controls->Add(this->groupBox32);
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Padding = System::Windows::Forms::Padding(3);
			this->tabPage7->Size = System::Drawing::Size(767, 316);
			this->tabPage7->TabIndex = 8;
			this->tabPage7->Text = L"Get S0";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// groupBox31
			// 
			this->groupBox31->Controls->Add(this->label108);
			this->groupBox31->Controls->Add(this->label107);
			this->groupBox31->Controls->Add(this->S0Zmag);
			this->groupBox31->Controls->Add(this->S0Ymag);
			this->groupBox31->Controls->Add(this->label106);
			this->groupBox31->Controls->Add(this->S0BoardTemp);
			this->groupBox31->Controls->Add(this->label105);
			this->groupBox31->Controls->Add(this->S0Temp);
			this->groupBox31->Controls->Add(this->label104);
			this->groupBox31->Controls->Add(this->S0Xmag);
			this->groupBox31->Controls->Add(this->S0XAccel);
			this->groupBox31->Controls->Add(this->S0YAccel);
			this->groupBox31->Controls->Add(this->S0ZAccel);
			this->groupBox31->Controls->Add(this->label101);
			this->groupBox31->Controls->Add(this->label102);
			this->groupBox31->Controls->Add(this->label103);
			this->groupBox31->Controls->Add(this->label86);
			this->groupBox31->Controls->Add(this->S0BitStatus);
			this->groupBox31->Controls->Add(this->S0Itow);
			this->groupBox31->Controls->Add(this->S0Xrate);
			this->groupBox31->Controls->Add(this->S0Yrate);
			this->groupBox31->Controls->Add(this->S0Zrate);
			this->groupBox31->Controls->Add(this->label92);
			this->groupBox31->Controls->Add(this->label94);
			this->groupBox31->Controls->Add(this->label99);
			this->groupBox31->Controls->Add(this->label100);
			this->groupBox31->Location = System::Drawing::Point(174, 12);
			this->groupBox31->Name = L"groupBox31";
			this->groupBox31->Size = System::Drawing::Size(427, 279);
			this->groupBox31->TabIndex = 25;
			this->groupBox31->TabStop = false;
			this->groupBox31->Text = L"Response";
			// 
			// label108
			// 
			this->label108->AutoSize = true;
			this->label108->Location = System::Drawing::Point(321, 94);
			this->label108->Name = L"label108";
			this->label108->Size = System::Drawing::Size(34, 13);
			this->label108->TabIndex = 38;
			this->label108->Text = L"Zmag";
			// 
			// label107
			// 
			this->label107->AutoSize = true;
			this->label107->Location = System::Drawing::Point(321, 62);
			this->label107->Name = L"label107";
			this->label107->Size = System::Drawing::Size(34, 13);
			this->label107->TabIndex = 37;
			this->label107->Text = L"Ymag";
			// 
			// S0Zmag
			// 
			this->S0Zmag->Location = System::Drawing::Point(210, 90);
			this->S0Zmag->Name = L"S0Zmag";
			this->S0Zmag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->S0Zmag->Size = System::Drawing::Size(100, 20);
			this->S0Zmag->TabIndex = 36;
			// 
			// S0Ymag
			// 
			this->S0Ymag->Location = System::Drawing::Point(210, 55);
			this->S0Ymag->Name = L"S0Ymag";
			this->S0Ymag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->S0Ymag->Size = System::Drawing::Size(100, 20);
			this->S0Ymag->TabIndex = 35;
			// 
			// label106
			// 
			this->label106->AutoSize = true;
			this->label106->Location = System::Drawing::Point(321, 170);
			this->label106->Name = L"label106";
			this->label106->Size = System::Drawing::Size(62, 13);
			this->label106->TabIndex = 34;
			this->label106->Text = L"BoardTemp";
			// 
			// S0BoardTemp
			// 
			this->S0BoardTemp->Location = System::Drawing::Point(210, 167);
			this->S0BoardTemp->Name = L"S0BoardTemp";
			this->S0BoardTemp->Size = System::Drawing::Size(102, 20);
			this->S0BoardTemp->TabIndex = 33;
			// 
			// label105
			// 
			this->label105->AutoSize = true;
			this->label105->Location = System::Drawing::Point(321, 144);
			this->label105->Name = L"label105";
			this->label105->Size = System::Drawing::Size(34, 13);
			this->label105->TabIndex = 32;
			this->label105->Text = L"Temp";
			// 
			// S0Temp
			// 
			this->S0Temp->Location = System::Drawing::Point(210, 137);
			this->S0Temp->Name = L"S0Temp";
			this->S0Temp->Size = System::Drawing::Size(100, 20);
			this->S0Temp->TabIndex = 31;
			// 
			// label104
			// 
			this->label104->AutoSize = true;
			this->label104->Location = System::Drawing::Point(321, 27);
			this->label104->Name = L"label104";
			this->label104->Size = System::Drawing::Size(34, 13);
			this->label104->TabIndex = 30;
			this->label104->Text = L"Xmag";
			// 
			// S0Xmag
			// 
			this->S0Xmag->Location = System::Drawing::Point(210, 19);
			this->S0Xmag->Name = L"S0Xmag";
			this->S0Xmag->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->S0Xmag->Size = System::Drawing::Size(100, 20);
			this->S0Xmag->TabIndex = 29;
			// 
			// S0XAccel
			// 
			this->S0XAccel->Location = System::Drawing::Point(20, 137);
			this->S0XAccel->Name = L"S0XAccel";
			this->S0XAccel->Size = System::Drawing::Size(100, 20);
			this->S0XAccel->TabIndex = 23;
			// 
			// S0YAccel
			// 
			this->S0YAccel->Location = System::Drawing::Point(20, 169);
			this->S0YAccel->Name = L"S0YAccel";
			this->S0YAccel->Size = System::Drawing::Size(100, 20);
			this->S0YAccel->TabIndex = 24;
			// 
			// S0ZAccel
			// 
			this->S0ZAccel->Location = System::Drawing::Point(20, 200);
			this->S0ZAccel->Name = L"S0ZAccel";
			this->S0ZAccel->Size = System::Drawing::Size(100, 20);
			this->S0ZAccel->TabIndex = 25;
			// 
			// label101
			// 
			this->label101->AutoSize = true;
			this->label101->Location = System::Drawing::Point(127, 203);
			this->label101->Name = L"label101";
			this->label101->Size = System::Drawing::Size(41, 13);
			this->label101->TabIndex = 28;
			this->label101->Text = L"ZAccel";
			// 
			// label102
			// 
			this->label102->AutoSize = true;
			this->label102->Location = System::Drawing::Point(128, 174);
			this->label102->Name = L"label102";
			this->label102->Size = System::Drawing::Size(41, 13);
			this->label102->TabIndex = 26;
			this->label102->Text = L"YAccel";
			// 
			// label103
			// 
			this->label103->AutoSize = true;
			this->label103->Location = System::Drawing::Point(128, 147);
			this->label103->Name = L"label103";
			this->label103->Size = System::Drawing::Size(41, 13);
			this->label103->TabIndex = 27;
			this->label103->Text = L"XAccel";
			// 
			// label86
			// 
			this->label86->AutoSize = true;
			this->label86->Location = System::Drawing::Point(321, 200);
			this->label86->Name = L"label86";
			this->label86->Size = System::Drawing::Size(52, 13);
			this->label86->TabIndex = 22;
			this->label86->Text = L"Bit Status";
			this->label86->Click += gcnew System::EventHandler(this, &Form1::label86_Click);
			// 
			// S0BitStatus
			// 
			this->S0BitStatus->Location = System::Drawing::Point(210, 196);
			this->S0BitStatus->Name = L"S0BitStatus";
			this->S0BitStatus->Size = System::Drawing::Size(100, 20);
			this->S0BitStatus->TabIndex = 21;
			// 
			// S0Itow
			// 
			this->S0Itow->Location = System::Drawing::Point(20, 19);
			this->S0Itow->Name = L"S0Itow";
			this->S0Itow->Size = System::Drawing::Size(100, 20);
			this->S0Itow->TabIndex = 0;
			// 
			// S0Xrate
			// 
			this->S0Xrate->Location = System::Drawing::Point(20, 47);
			this->S0Xrate->Name = L"S0Xrate";
			this->S0Xrate->Size = System::Drawing::Size(100, 20);
			this->S0Xrate->TabIndex = 2;
			// 
			// S0Yrate
			// 
			this->S0Yrate->Location = System::Drawing::Point(20, 79);
			this->S0Yrate->Name = L"S0Yrate";
			this->S0Yrate->Size = System::Drawing::Size(100, 20);
			this->S0Yrate->TabIndex = 3;
			// 
			// S0Zrate
			// 
			this->S0Zrate->Location = System::Drawing::Point(20, 110);
			this->S0Zrate->Name = L"S0Zrate";
			this->S0Zrate->Size = System::Drawing::Size(100, 20);
			this->S0Zrate->TabIndex = 4;
			// 
			// label92
			// 
			this->label92->AutoSize = true;
			this->label92->Location = System::Drawing::Point(127, 113);
			this->label92->Name = L"label92";
			this->label92->Size = System::Drawing::Size(37, 13);
			this->label92->TabIndex = 11;
			this->label92->Text = L"ZRate";
			// 
			// label94
			// 
			this->label94->AutoSize = true;
			this->label94->Location = System::Drawing::Point(126, 22);
			this->label94->Name = L"label94";
			this->label94->Size = System::Drawing::Size(36, 13);
			this->label94->TabIndex = 10;
			this->label94->Text = L"ITOW";
			// 
			// label99
			// 
			this->label99->AutoSize = true;
			this->label99->Location = System::Drawing::Point(128, 84);
			this->label99->Name = L"label99";
			this->label99->Size = System::Drawing::Size(32, 13);
			this->label99->TabIndex = 7;
			this->label99->Text = L"Yrate";
			// 
			// label100
			// 
			this->label100->AutoSize = true;
			this->label100->Location = System::Drawing::Point(128, 57);
			this->label100->Name = L"label100";
			this->label100->Size = System::Drawing::Size(37, 13);
			this->label100->TabIndex = 8;
			this->label100->Text = L"XRate";
			// 
			// groupBox32
			// 
			this->groupBox32->Controls->Add(this->button1);
			this->groupBox32->Location = System::Drawing::Point(19, 14);
			this->groupBox32->Name = L"groupBox32";
			this->groupBox32->Size = System::Drawing::Size(138, 277);
			this->groupBox32->TabIndex = 24;
			this->groupBox32->TabStop = false;
			this->groupBox32->Text = L"Command";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(10, 29);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(100, 25);
			this->button1->TabIndex = 16;
			this->button1->Text = L"Send";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_3);
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->groupBox22);
			this->tabPage4->Controls->Add(this->groupBox21);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(767, 316);
			this->tabPage4->TabIndex = 6;
			this->tabPage4->Text = L"UserPing";
			this->tabPage4->UseVisualStyleBackColor = true;
			this->tabPage4->Click += gcnew System::EventHandler(this, &Form1::tabPage4_Click);
			this->tabPage4->Enter += gcnew System::EventHandler(this, &Form1::tabPage4_Enter);
			// 
			// groupBox22
			// 
			this->groupBox22->Controls->Add(this->UsrPingResponse);
			this->groupBox22->Location = System::Drawing::Point(174, 18);
			this->groupBox22->Name = L"groupBox22";
			this->groupBox22->Size = System::Drawing::Size(543, 279);
			this->groupBox22->TabIndex = 21;
			this->groupBox22->TabStop = false;
			this->groupBox22->Text = L"Response";
			// 
			// UsrPingResponse
			// 
			this->UsrPingResponse->Location = System::Drawing::Point(18, 32);
			this->UsrPingResponse->Name = L"UsrPingResponse";
			this->UsrPingResponse->Size = System::Drawing::Size(501, 20);
			this->UsrPingResponse->TabIndex = 0;
			// 
			// groupBox21
			// 
			this->groupBox21->Controls->Add(this->button17);
			this->groupBox21->Controls->Add(this->UsrPingSend);
			this->groupBox21->Location = System::Drawing::Point(19, 20);
			this->groupBox21->Name = L"groupBox21";
			this->groupBox21->Size = System::Drawing::Size(138, 277);
			this->groupBox21->TabIndex = 20;
			this->groupBox21->TabStop = false;
			this->groupBox21->Text = L"Command";
			// 
			// button17
			// 
			this->button17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button17->Location = System::Drawing::Point(10, 84);
			this->button17->Name = L"button17";
			this->button17->Size = System::Drawing::Size(100, 25);
			this->button17->TabIndex = 17;
			this->button17->Text = L"Get Version";
			this->button17->UseVisualStyleBackColor = true;
			this->button17->Click += gcnew System::EventHandler(this, &Form1::button17_Click);
			// 
			// UsrPingSend
			// 
			this->UsrPingSend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->UsrPingSend->Location = System::Drawing::Point(10, 29);
			this->UsrPingSend->Name = L"UsrPingSend";
			this->UsrPingSend->Size = System::Drawing::Size(100, 25);
			this->UsrPingSend->TabIndex = 16;
			this->UsrPingSend->Text = L"Send Ping";
			this->UsrPingSend->UseVisualStyleBackColor = true;
			this->UsrPingSend->Click += gcnew System::EventHandler(this, &Form1::A6Send_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox25);
			this->tabPage2->Controls->Add(this->groupBox26);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(767, 316);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Ping";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox25
			// 
			this->groupBox25->Controls->Add(this->label90);
			this->groupBox25->Controls->Add(this->PingResponse);
			this->groupBox25->Location = System::Drawing::Point(192, 19);
			this->groupBox25->Name = L"groupBox25";
			this->groupBox25->Size = System::Drawing::Size(314, 281);
			this->groupBox25->TabIndex = 22;
			this->groupBox25->TabStop = false;
			this->groupBox25->Text = L"Response";
			// 
			// label90
			// 
			this->label90->AutoSize = true;
			this->label90->Location = System::Drawing::Point(266, 31);
			this->label90->Name = L"label90";
			this->label90->Size = System::Drawing::Size(37, 13);
			this->label90->TabIndex = 11;
			this->label90->Text = L"Status";
			// 
			// PingResponse
			// 
			this->PingResponse->Location = System::Drawing::Point(12, 24);
			this->PingResponse->Name = L"PingResponse";
			this->PingResponse->Size = System::Drawing::Size(236, 20);
			this->PingResponse->TabIndex = 6;
			// 
			// groupBox26
			// 
			this->groupBox26->Controls->Add(this->StartSend);
			this->groupBox26->Location = System::Drawing::Point(25, 19);
			this->groupBox26->Name = L"groupBox26";
			this->groupBox26->Size = System::Drawing::Size(139, 281);
			this->groupBox26->TabIndex = 21;
			this->groupBox26->TabStop = false;
			this->groupBox26->Text = L"Command";
			// 
			// StartSend
			// 
			this->StartSend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->StartSend->Location = System::Drawing::Point(10, 29);
			this->StartSend->Name = L"StartSend";
			this->StartSend->Size = System::Drawing::Size(100, 25);
			this->StartSend->TabIndex = 16;
			this->StartSend->Text = L"Send";
			this->StartSend->UseVisualStyleBackColor = true;
			this->StartSend->Click += gcnew System::EventHandler(this, &Form1::StartSend_Click);
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBox23);
			this->tabPage1->Controls->Add(this->groupBox24);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(767, 316);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Reset";
			this->tabPage1->UseVisualStyleBackColor = true;
			this->tabPage1->Enter += gcnew System::EventHandler(this, &Form1::tabPage1_Enter);
			// 
			// groupBox23
			// 
			this->groupBox23->Controls->Add(this->label77);
			this->groupBox23->Controls->Add(this->SwResetResponse);
			this->groupBox23->Location = System::Drawing::Point(184, 20);
			this->groupBox23->Name = L"groupBox23";
			this->groupBox23->Size = System::Drawing::Size(178, 277);
			this->groupBox23->TabIndex = 20;
			this->groupBox23->TabStop = false;
			this->groupBox23->Text = L"Response";
			// 
			// label77
			// 
			this->label77->AutoSize = true;
			this->label77->Location = System::Drawing::Point(134, 41);
			this->label77->Name = L"label77";
			this->label77->Size = System::Drawing::Size(37, 13);
			this->label77->TabIndex = 17;
			this->label77->Text = L"Result";
			// 
			// SwResetResponse
			// 
			this->SwResetResponse->Location = System::Drawing::Point(28, 34);
			this->SwResetResponse->Name = L"SwResetResponse";
			this->SwResetResponse->Size = System::Drawing::Size(100, 20);
			this->SwResetResponse->TabIndex = 14;
			// 
			// groupBox24
			// 
			this->groupBox24->Controls->Add(this->button23);
			this->groupBox24->Controls->Add(this->SwResetSend);
			this->groupBox24->Location = System::Drawing::Point(20, 20);
			this->groupBox24->Name = L"groupBox24";
			this->groupBox24->Size = System::Drawing::Size(138, 277);
			this->groupBox24->TabIndex = 19;
			this->groupBox24->TabStop = false;
			this->groupBox24->Text = L"Command";
			// 
			// button23
			// 
			this->button23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button23->Location = System::Drawing::Point(19, 126);
			this->button23->Name = L"button23";
			this->button23->Size = System::Drawing::Size(100, 25);
			this->button23->TabIndex = 17;
			this->button23->Text = L"User Reset";
			this->button23->UseVisualStyleBackColor = true;
			this->button23->Click += gcnew System::EventHandler(this, &Form1::button23_Click);
			// 
			// SwResetSend
			// 
			this->SwResetSend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SwResetSend->Location = System::Drawing::Point(10, 29);
			this->SwResetSend->Name = L"SwResetSend";
			this->SwResetSend->Size = System::Drawing::Size(109, 25);
			this->SwResetSend->TabIndex = 16;
			this->SwResetSend->Text = L"Platform Reset";
			this->SwResetSend->UseVisualStyleBackColor = true;
			this->SwResetSend->Click += gcnew System::EventHandler(this, &Form1::SwResetSend_Click);
			// 
			// AnavTab
			// 
			this->AnavTab->Controls->Add(this->tabPage1);
			this->AnavTab->Controls->Add(this->tabPage2);
			this->AnavTab->Controls->Add(this->tabPage4);
			this->AnavTab->Controls->Add(this->tabPage7);
			this->AnavTab->Controls->Add(this->tabPage5);
			this->AnavTab->Controls->Add(this->tabPage6);
			this->AnavTab->Controls->Add(this->SetFields);
			this->AnavTab->Controls->Add(this->tabPage9);
			this->AnavTab->Controls->Add(this->tabPage10);
			this->AnavTab->Controls->Add(this->tabPage13);
			this->AnavTab->Enabled = false;
			this->AnavTab->Location = System::Drawing::Point(25, 173);
			this->AnavTab->Name = L"AnavTab";
			this->AnavTab->SelectedIndex = 0;
			this->AnavTab->Size = System::Drawing::Size(775, 342);
			this->AnavTab->TabIndex = 29;
			this->AnavTab->Selecting += gcnew System::Windows::Forms::TabControlCancelEventHandler(this, &Form1::AnavTab_Selecting_1);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(810, 562);
			this->Controls->Add(this->AnavTab);
			this->Controls->Add(this->groupBox20);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"OpenIMU Test Utility";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->groupBox20->ResumeLayout(false);
			this->groupBox20->PerformLayout();
			this->tabPage13->ResumeLayout(false);
			this->groupBox17->ResumeLayout(false);
			this->groupBox17->PerformLayout();
			this->groupBox18->ResumeLayout(false);
			this->groupBox18->PerformLayout();
			this->tabPage10->ResumeLayout(false);
			this->groupBox13->ResumeLayout(false);
			this->groupBox13->PerformLayout();
			this->groupBox14->ResumeLayout(false);
			this->tabPage9->ResumeLayout(false);
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->groupBox12->ResumeLayout(false);
			this->SetFields->ResumeLayout(false);
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox8->ResumeLayout(false);
			this->tabPage6->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->tabPage5->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->tabPage7->ResumeLayout(false);
			this->groupBox31->ResumeLayout(false);
			this->groupBox31->PerformLayout();
			this->groupBox32->ResumeLayout(false);
			this->tabPage4->ResumeLayout(false);
			this->groupBox22->ResumeLayout(false);
			this->groupBox22->PerformLayout();
			this->groupBox21->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->groupBox25->ResumeLayout(false);
			this->groupBox25->PerformLayout();
			this->groupBox26->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBox23->ResumeLayout(false);
			this->groupBox23->PerformLayout();
			this->groupBox24->ResumeLayout(false);
			this->AnavTab->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void textBox1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void exitToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void exitToolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
				Close();
		 }
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			int n = 0;
		 }

private: System::Void toolStripMenuItemList_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox4_Enter(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void label41_Click(System::Object^  sender, System::EventArgs^  e) {
}


private: System::Void OpenCom_Click(System::Object^  sender, System::EventArgs^  e) {
			 char* portName = (char*)(Marshal::StringToHGlobalAnsi(comboBox1->Text)).ToPointer();
			 int rate = Convert::ToInt32(baudRate->Text);
			 if (_stream) {
				 return;		// already opened
			  }
			 _stream = new SerialStream(portName, rate);
			 if (!_stream->isValid()){
				 delete(_stream);
				 _stream = NULL;
				 return;
			 }
//			 quietMode->Enabled = true;
			 AnavTab->Enabled = true;
			 _protocol->init(_stream);
}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void Reset_Click(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void textBox14_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void LedCtl_Click(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void Bit3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void ClearErrors_Click(System::Object^  sender, System::EventArgs^  e) {
			 _errorCounter = 0;
			 _cmdCounter = 0;
//			 cmdCnt->Text = "";
			 errorLabel->Visible = false;
}
private: System::Void LogRunTime_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void PingStop_Click(System::Object^  sender, System::EventArgs^  e) {
			 _pingRunning = false;
}
private: System::Void tabControl1_Deselected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e) {
			 stopPing();
}
private: System::Void PingModeTime_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button1_Click_2(System::Object^  sender, System::EventArgs^  e) {
			 uint16_t fieldId = 1, fieldData = 0;
			 int crc;

			 XbowSetFieldsCmdPacket pkt(1, &fieldId, &fieldData);
//			 quietMode->Enabled = false;

			 pkt.send(_stream);
			 uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | SF, &crc);
			 if (msg){
				 XbowSetFieldsResponsePacket resp(msg);
				 crc = 0;
			 }
			 else{
				 Sleep(3);
			 }
//			 quietMode->Enabled = true;

}
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
//			 PavamStatus->Visible = true;
//			 PavamBits->Visible = true;
//			 AnavTab->Enabled = false;
			 AnavTab->Visible  = false;
}

private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 stopPing();
//			 PavamStatus->Visible = false;
//			 PavamBits->Visible = false;
//			 PavamTab->Enabled = false;
//			 AnavTab->Enabled  = true;
			 AnavTab->Visible  = true;
}

private: System::Void label91_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void SwResetSend_Click(System::Object^  sender, System::EventArgs^  e) {

			 int crc;
			 XbowSoftwareResetCmdPacket pkt;
			 SwResetResponse->Text = "";
			 pkt.send(_stream);
			 uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | SR, &crc);
			 if (msg){
				 SwResetResponse->Text = "OK";
				 crc = 0;
			 }
			 else{
				 Sleep(3);
				 SwResetResponse->Text = "Failed";
			 }
}
private: System::Void StartSend_Click(System::Object^  sender, System::EventArgs^  e) {

	int crc;
	XbowPingPacket pkt;

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | PK, &crc);
	if (msg) {
		PingResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		PingResponse->Text = "Failed";
	}


}
private: System::Void FinishTimeTag_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void A6Send_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc, len;
	OpenIMUPingPacket pkt;

	UsrPingResponse->Text = "";
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_PING, &crc, &len);
	if (msg) {
		msg[len] = 0;
		String ^sss = gcnew String((char*)msg);
		UsrPingResponse->Text = sss;
		crc = 0;
	}
	else {
		Sleep(3);
		UsrPingResponse->Text = "Failed";
	}
}
private: System::Void label86_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button1_Click_3(System::Object^  sender, System::EventArgs^  e) {
			 int crc;

			 XbowGetPacket pkt(S0);

			 pkt.send(_stream);
			 uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | S0, &crc);
			 if (msg){
				 XbowSensorScaled0Pkt resp(msg, 30);
				 S0Xrate->Text = (resp._rate[0] * 1260/pow(2,16)).ToString("0.###");
				 S0Yrate->Text = (resp._rate[1] * 1260 / pow(2, 16)).ToString("0.###");
				 S0Zrate->Text = (resp._rate[2] * 1260 / pow(2, 16)).ToString("0.###");
				 S0XAccel->Text = (resp._accel[0] * 20 /pow(2,16)).ToString("0.###");
				 S0YAccel->Text = (resp._accel[1] * 20 / pow(2, 16)).ToString("0.###");
				 S0ZAccel->Text = (resp._accel[2] * 20 / pow(2, 16)).ToString("0.###");
				 S0Xmag->Text   = (resp._mag[0] * 2 / pow(2, 16)).ToString("0.###");
				 S0Ymag->Text   = (resp._mag[1] * 2 / pow(2, 16)).ToString("0.###");
				 S0Zmag->Text   = (resp._mag[2] * 2 / pow(2, 16)).ToString("0.###");
				 S0BitStatus->Text = (resp._bitStatus).ToString();
				 S0Temp->Text = (resp._rateTemp[0] * 200 / pow(2, 16)).ToString("0.###");
				 S0BoardTemp->Text = (resp._boardTemp * 200 / pow(2, 16)).ToString("0.###");
				 S0Itow->Text = (resp._gpsItow).ToString();
			 }
			 else{
				 Sleep(3);
			 }
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void A6Roll_TextChanged(System::Object^  sender,System::EventArgs^  e) {
}
private: System::Void label127_Click(System::Object^  sender, System::EventArgs^  e) {
}

private: System::Void label132_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void CalFileName_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}




private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
}
private: System::Void AnavTab_Selecting(System::Object^  sender, System::Windows::Forms::TabControlCancelEventArgs^  e) {
	SwResetResponse->Text = "";
}
private: System::Void AnavTab_Selecting_1(System::Object^  sender, System::Windows::Forms::TabControlCancelEventArgs^  e) {
	PingResponse->Text = "";
}
private: System::Void label10_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) {
	int crc;
}
private: System::Void button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
	int crc;
	OpenIMUGetPacket pkt(USR_OUT_TEST);

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_OUT_TEST, &crc);
	if (msg) {
		getUTResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		getUTResponse->Text = "Failed";
	}

}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc;
	XbowGetPacket pkt(USR_OUT_DATA1);

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_OUT_DATA1, &crc);
	if (msg) {
		OpenIMUData1Pkt resp(msg, 30);
		data1Xrate->Text  = System::Convert::ToString((double)resp._rate[0]);
		data1Yrate->Text  = System::Convert::ToString((double)resp._rate[1]);
		data1Zrate->Text = System::Convert::ToString((double)resp._rate[2]);
		data1Xaccel->Text = System::Convert::ToString((double)resp._accel[0]);
		data1Yaccel->Text = System::Convert::ToString((double)resp._accel[1]);
		data1Zaccel->Text = System::Convert::ToString((double)resp._accel[2]);
		data1Xmag->Text   = System::Convert::ToString((double)resp._mag[0]);
		data1Ymag->Text   = System::Convert::ToString((double)resp._mag[1]);
		data1Zmag->Text   = System::Convert::ToString((double)resp._mag[2]);
		data1Tstamp->Text = System::Convert::ToString(resp._tStamp);
		d1Res->Text = "Success";
	}
	else {
		Sleep(3);
		d1Res->Text = "Failed";
	}

}
private: System::Void setPacketRate_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Rate
	int crc;
	uint16_t offset    = 4;
	uint64_t rate = System::Convert::ToUInt32(usrPacketRate->Text);
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetParamCmdPacket pkt(offset, rate);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_PARAM, &crc);
	if (msg) {
		OpenIMUSetParamResponsePacket pkt(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (pkt._numFields).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}
}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	// Baud Rate
	int crc;
	uint16_t offset = 2;
	uint16_t numParams = 1;
	uint64_t rate = System::Convert::ToUInt32(usrBaudRate->Text);
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetParamCmdPacket pkt(offset, rate);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_PARAM, &crc);
	if (msg) {
		OpenIMUSetParamsResponsePacket pkt(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (pkt._numFields).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	char str[20];
	uint16_t offset = 3;
	uint16_t numParams = 1;
	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(usrPacketType->Text);
	uint64_t type = 0;
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	memcpy((void*)&type, str2, 8);
	type &= 0xffffffff;
	OpenIMUSetParamCmdPacket pkt(offset, type);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_PARAM, &crc);
	if (msg) {
		OpenIMUSetParamsResponsePacket pkt(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (pkt._numFields).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	uint16_t offset = 5;
	uint16_t numParams = 1;
	uint64_t type = System::Convert::ToUInt32(usrAccelLpf->Text);
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetParamsCmdPacket pkt(numParams, offset, &type);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_CONFIG, &crc);
	if (msg) {
		OpenIMUSetParamsResponsePacket p(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (p._numFields).ToString();
		if (p._numFields != 0) {
			sparamVal->Text = (p._params[0]).ToString();
		}
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	uint16_t offset = 6;
	uint16_t numParams = 1;
	uint64_t type = System::Convert::ToUInt32(usrRateLpf->Text);
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetParamsCmdPacket pkt(numParams, offset, &type);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_CONFIG, &crc);
	if (msg) {
		OpenIMUSetParamsResponsePacket p(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (p._numFields).ToString();
		if (p._numFields != 0) {
			sparamVal->Text = (p._params[0]).ToString();
		}
	}
	else {
		sparamRes->Text = "Failed";
	}


}
private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	uint16_t offset    = 2;
	uint16_t numParams = 6;
	uint32_t type = System::Convert::ToUInt32(usrRateLpf->Text);
	char str[100];
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	unitBaudRate->Text = "";
	unitPacketType->Text = "";
	unitPacketRate->Text = "";
	unitAccelLpf->Text = "";
	unitGyroLpf->Text = "";
	unitOrientation->Text = "";
	OpenIMUGetParamsCmdPacket pkt(numParams, offset);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_GET_CONFIG, &crc);
	if (msg) {
		OpenIMUGetParamsResponsePacket p(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (p._numFields).ToString();
		if (p._numFields != 0) {
			unitBaudRate->Text = ((int)p._params[0]).ToString();
			memcpy(str, (char*)(&p._params[1]), 8);
			String ^sss = gcnew String(str);
			unitPacketType->Text = sss;
			unitPacketRate->Text = ((int)p._params[2]).ToString();
			unitAccelLpf->Text = ((int)p._params[3]).ToString();
			unitGyroLpf->Text = ((int)p._params[4]).ToString();
			memcpy(str, (char*)(&p._params[5]), 8);
			sss = gcnew String(str);
			unitOrientation->Text = sss;
		}
	}
	else {
		sparamRes->Text = "Failed";
	}
}
private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc;
	OpenIMUSaveConfigCmdPacket pkt;
	saveResponse->Text = "";

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_SAVE_CONFIG, &crc);
	if (msg) {
		saveResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		saveResponse->Text = "Failed";
	}

}

private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {

	AnavTab->Enabled = false;
	_protocol->disconnect();
	_stream = NULL;
}

private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc;
	OpenIMURestoreDefaultsCmdPacket pkt;
	saveResponse->Text = "";

	pkt.send(_stream);	
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_RESTORE_DEFAULTS, &crc);
	if (msg) {
		defaultsResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		defaultsResponse->Text = "Failed";
	}

}
private: System::Void lockEEprom_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void unlockEEprom_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void getRate_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	uint16_t offset = 4;
	uint16_t numParams = 6;
	uint32_t type = System::Convert::ToUInt32(usrRateLpf->Text);
	char str[100];
	unitPacketRate->Text = "";
	OpenIMUGetParamCmdPacket pkt(offset);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_GET_PARAM, &crc);
	if (msg) {
		OpenIMUGetParamResponsePacket p(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (p._offset).ToString();
		unitPacketRate->Text = ((int)p._param).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button14_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Type
	int crc;
	int len;
	uint16_t offset = 2;
	uint16_t numParams = 6;
	uint32_t type = System::Convert::ToUInt32(usrRateLpf->Text);
	char str[100];
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	unitBaudRate->Text = "";
	unitPacketType->Text = "";
	unitPacketRate->Text = "";
	unitAccelLpf->Text = "";
	unitGyroLpf->Text = "";
	unitOrientation->Text = "";
	OpenIMUGetAllCmdPacket pkt;
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_GET_ALL, &crc, &len);
	if (msg) {
		OpenIMUGetAllResponsePacket p(msg, len);
		sparamRes->Text = "Success";
		sparamNum->Text = ((int)p._len).ToString();
		if (len != 0) {
			unitBaudRate->Text = ((int)p._params[2]).ToString();
			memcpy(str, (char*)(&p._params[3]), 8);
			String ^sss = gcnew String(str);
			unitPacketType->Text = sss;
			unitPacketRate->Text = ((int)p._params[4]).ToString();
			unitAccelLpf->Text = ((int)p._params[5]).ToString();
			unitGyroLpf->Text = ((int)p._params[6]).ToString();
			memcpy(str, (char*)(&p._params[7]), 8);
			sss = gcnew String(str);
			unitOrientation->Text = sss;
		}
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button15_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Rate
	int crc;
	uint16_t offset = 2;
	uint16_t numParams = 5;
	uint64_t params[MAX_PARAMS];

	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(usrPacketType->Text);
	uint64_t type = 0;
	memcpy((void*)&type, str2, 8);
	type &= 0xffffffff;
	params[0] = System::Convert::ToUInt32(usrBaudRate->Text);
	params[1] = type;
	params[2] = System::Convert::ToUInt32(usrPacketRate->Text);
	params[3] = System::Convert::ToUInt32(usrAccelLpf->Text);
	params[4] = System::Convert::ToUInt32(usrRateLpf->Text);


	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetParamsCmdPacket pkt(numParams, offset, params);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_CONFIG, &crc);
	if (msg) {
		OpenIMUSetParamsResponsePacket pkt(msg);
		sparamRes->Text = "Success";
		sparamNum->Text = (pkt._numFields).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}

}
private: System::Void button16_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Rate
	int crc;
	int pldLen;
	uint16_t numParams = 7;
	uint64_t params[MAX_PARAMS];


	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(usrPacketType->Text);
	uint64_t type = 0;
	memcpy((void*)&type, str2, 8);
	type &= 0xffffffff;
	params[0] = 0;
	params[1] = 0;
	params[2] = System::Convert::ToUInt32(usrBaudRate->Text);
	params[3] = type;
	params[4] = System::Convert::ToUInt32(usrPacketRate->Text);
	params[5] = System::Convert::ToUInt32(usrAccelLpf->Text);
	params[6] = System::Convert::ToUInt32(usrRateLpf->Text);


	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";
	OpenIMUSetAllCmdPacket pkt(numParams, params);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_UPDATE_ALL, &crc, &pldLen);
	if (msg) {
		OpenIMUSetAllResponsePacket p(msg, pldLen);
		sparamRes->Text = "Success";
		sparamNum->Text = (pkt._numFields).ToString();
	}
	else {
		sparamRes->Text = "Failed";
	}


}
private: System::Void tabPage4_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void tabPage1_Enter(System::Object^  sender, System::EventArgs^  e) {
	SwResetResponse->Text = "";
}
private: System::Void tabPage4_Enter(System::Object^  sender, System::EventArgs^  e) {
	UsrPingResponse->Text = "";
}
private: System::Void tabPage5_Enter(System::Object^  sender, System::EventArgs^  e) {
	getUTResponse->Text = "";
}
private: System::Void SetFields_Enter(System::Object^  sender, System::EventArgs^  e) {
	sparamRes->Text = "";
	sparamNum->Text = "";
	sparamVal->Text = "";

}
private: System::Void button17_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc, len;
	OpenIMUVersionPacket pkt;

	UsrPingResponse->Text = "";
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_GET_VERSION, &crc, &len);
	if (msg) {
		msg[len] = 0;
		String ^sss = gcnew String((char*)msg);
		UsrPingResponse->Text = sss;
		crc = 0;
	}
	else {
		Sleep(3);
		UsrPingResponse->Text = "Failed";
	}

}
private: System::Void button18_Click(System::Object^  sender, System::EventArgs^  e) {

	int crc;
	XbowJIPacket pkt;

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | JI, &crc);
	if (msg) {
		PingResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		PingResponse->Text = "Failed";
	}



}
private: System::Void button19_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc;
	XbowJAPacket pkt;

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | JA, &crc);
	if (msg) {
		PingResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		PingResponse->Text = "Failed";
	}

}
private: System::Void button20_Click(System::Object^  sender, System::EventArgs^  e) {
	// Packet Rate
	int crc;
	uint32_t offset = 0;
	uint32_t num    = 0;

	OpenIMUWACmdPacket pkt(offset, num);
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | WA, &crc, NULL, 10);
	if (msg) {
		UsrPingResponse->Text = "OK";
	}
	else {
		UsrPingResponse->Text = "Failed";
	}

}
private: System::Void button21_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc, len;
	XbowGetPacket pkt(USR_OUT_DATA2);

	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_OUT_DATA2, &crc, &len);
	if (msg) {
		OpenIMUData2Pkt resp(msg, len);
		data2Tstamp->Text = System::Convert::ToString((uint32_t)resp._tStamp);
		data2Char->Text   = System::Convert::ToString((char)resp._byte);
		data2Short->Text = System::Convert::ToString(resp._short);
		data2Int->Text = System::Convert::ToString(resp._int);
		data2LL->Text = System::Convert::ToString((uint64_t)resp._ll);
		data2Double->Text = System::Convert::ToString(resp._dbl);
		d2Resp->Text = "Success";
	}
	else {
		Sleep(3);
		d2Resp->Text = "Failed";
	}


}
private: System::Void button22_Click(System::Object^  sender, System::EventArgs^  e) {
	int crc, len;
	int rates[] = { 38400, 57600, 115200 };
	char* portName = (char*)(Marshal::StringToHGlobalAnsi(comboBox1->Text)).ToPointer();

	if (_stream) {
		_stream->close();
		delete(_stream);
		_stream = NULL;
	}

	OpenIMUPingPacket pkt;

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 3; j++) {
			_stream = new SerialStream(portName, rates[j]);
			if (!_stream->isValid()) {
				delete(_stream);
				_stream = NULL;
				PingResponse->Text = "Crushed";
				AnavTab->Enabled = false;
				return;
			}
			UsrPingResponse->Text = "";
			pkt.send(_stream);
			uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_PING, &crc, &len);
			if (msg) {
				msg[len] = 0;
				String ^sss = gcnew String((char*)msg);
				PingResponse->Text = sss;
				crc = 0;
			}
			else {
				Sleep(3);
				PingResponse->Text = "Failed";
			}
			_stream->close();
		}

	}

}
private: System::Void button23_Click(System::Object^  sender, System::EventArgs^  e) {

	int crc;
	OpenIMUResetCmdPacket pkt;
	SwResetResponse->Text = "";
	pkt.send(_stream);
	uint8_t *msg = _protocol->getXbowMessage(_stream, (XBOW_PROTOCOL << 16) | USR_IN_RESET, &crc);
	if (msg) {
		SwResetResponse->Text = "OK";
		crc = 0;
	}
	else {
		Sleep(3);
		SwResetResponse->Text = "Failed";
	}

}
};

public ref class MyThreadClass
{
private:
	Form1^				_myForm;
	TestProto		*_protocol;
	SerialStream		*_stream;
	int					_frequency;
	int				    _running;
public:
	MyThreadClass(Form1^ myForm, SerialStream  *stream)
	{
		_myForm = myForm;
		 TestProto	*_protocol = new TestProto;
		_stream = stream;
		_frequency = 0;
		_running   = 0;
	}

	void setFrequency(int freq){
		_frequency = freq;
	}

	void terminate(){
		_running = 0;
	}

	void Run();
	/*
	{
		int sent = 0, rcvd = 0;
		int crc,      delay = 1000/_frequency;
		sent++;
		do{
			DrsPavamPingCmdPacket p;
			p.send(_stream);
			uint8_t *msg = _protocol->getMessage(_stream, (DRS_RESPONSE_PROTOCOL << 16) | DRS_PING_RESPONSE, &crc);
			if (msg){
				rcvd++;
				DrsPavamPingResponsePacket pkt(msg, 14);
				_myForm->Invoke(_myForm->_myDelegate, pkt._modeTime, pkt._bitWord, pkt._statusWord, pkt._sysTime, pkt._temp, sent, rcvd);
			}
			if (!_frequency){
				break;
			}
			int	delay = 1000 /_frequency;
			Sleep(delay);
		} while (_running);
	}*/
};


void MyThreadClass::Run()
{
	int sent = 0, rcvd = 0, start, elapsed;
	int crc;
	_frequency = _myForm->pingFrequency();

	do{
		sent++;
//		PingCmdPacket p;
//		p.send(_stream);
//		start = clock();
		uint8_t *msg = _protocol->getMessage(_stream, (XBOW_PROTOCOL << 16) | PK, &crc);
		if (msg){
			elapsed = clock() - start;
			rcvd++;
//			DrsPavamPingResponsePacket pkt(msg, 14);
//			if (pkt._modeTime > 600000){
//				Sleep(5000);
//			}
//			_myForm->Invoke(_myForm->_myDelegate, pkt._modeTime, pkt._bitWord, pkt._statusWord, pkt._sysTime, pkt._temp, sent, rcvd, elapsed);
		}
		if (!_frequency){
			break;
		}
		int	delay = 1000 / _frequency;
		Sleep(delay);
	} while (_myForm->isThreadRunning());
	
	_myForm->Invoke(_myForm->_enablePingDelegate, sent, rcvd);

}


void Form1::ThreadFunction()
{
	MyThreadClass^ myThreadClassObject = gcnew MyThreadClass(this, _stream);
	myThreadClassObject->Run();
}

void Form1::stopPing()
{
}


}