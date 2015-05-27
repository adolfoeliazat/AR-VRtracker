using System;
using System.Runtime.InteropServices;
using UnityEngine;
using System.Collections;

public static class LocomotionDevice
{
	[StructLayout(LayoutKind.Sequential, Pack=1)]
	private struct dataexchange_t {
		float qr_w, qr_x, qr_y, qr_z; //16 bytes
		Int16 mag_x, mag_y, mag_z; //6 bytes
		Int16 acc_x, acc_y, acc_z;
		Int16 gyr_x, gyr_y, gyr_z;
		float time_period; //4 bytes
		float d1, d2, d3, d4, d5;
		Byte d6;
		Byte btn; //1 byte

		public byte Btn { 
			get {return btn;}
			//private set {var = 0;}
		}

		public float Qr_w { 
			get {return qr_w;}
			//private set {var = 0;}
		}
		public float Qr_x { 
			get {return qr_x;}
			//private set {var = 0;}
		}
		public float Qr_y { 
			get {return qr_y;}
			//private set {var = 0;}
		}
		public float Qr_z { 
			get {return qr_z;}
			//private set {var = 0;}
		}
	};
	//note: auto alignment disabled

	private static byte prev_btn_state;

	public enum Btns
	{
		Front = 1,
		Side = 2,
		Top = 4
	};	

	private const byte COMLINK_SIGNATURE = 0x3C;
	private const string COMLINK_NAME = "MyFileMappingObject";
	private static dataexchange_t data;
	
	private enum FileProtection : uint
	{
		ReadOnly = 0x0002,
		ReadWrite = 0x0004
	}
	
	private enum FileRights : uint
	{
		Read = 0x0004,
		Write = 0x0002,
		AllAccess = 0x001f,
		ReadWrite = Read + Write
	}
	
	private static IntPtr ShMemFileHandler, IPCMapPntr;
	
	[DllImport ("kernel32.dll", SetLastError = true)]
	private static extern IntPtr CreateFileMapping(IntPtr hFile, int lpAttributes, FileProtection flProtect, uint dwMaximumSizeHigh, uint dwMaximumSizeLow, string lpName);
	
	[DllImport ("kernel32.dll", SetLastError=true)]
	private static extern IntPtr OpenFileMapping(FileRights dwDesiredAccess, bool bInheritHandle, string lpName);
	
	[DllImport ("kernel32.dll", SetLastError = true)]
	private static extern IntPtr MapViewOfFile(IntPtr hFileMappingObject, FileRights dwDesiredAccess, uint dwFileOffsetHigh, uint dwFileOffsetLow, uint dwNumberOfBytesToMap);
	
	[DllImport ("Kernel32.dll")]
	private static extern bool UnmapViewOfFile(IntPtr map);
	
	[DllImport ("kernel32.dll")]
	private static extern int CloseHandle(IntPtr hObject);

	public static bool ConnenctToMemFile()
	{
		ShMemFileHandler = OpenFileMapping(FileRights.AllAccess, false, COMLINK_NAME);
		if (ShMemFileHandler == IntPtr.Zero) {
			Debug.LogError ("No fHandler");
			return false;
		}
		
		IPCMapPntr = MapViewOfFile(ShMemFileHandler, FileRights.AllAccess, 0, 0, 256);
		if (IPCMapPntr == IntPtr.Zero) {
			Debug.LogError ("No fMap");
			return false;
		}

		//initial reading
		data = (LocomotionDevice.dataexchange_t)Marshal.PtrToStructure (IPCMapPntr, typeof(LocomotionDevice.dataexchange_t));		
		prev_btn_state = data.Btn;

		return true;
	}

	public static bool DisconnecFromMemFile()
	{
		CloseHandle(ShMemFileHandler);
		return true;
	}

	public static bool ReadHidDevice()
	{
		if (IPCMapPntr == IntPtr.Zero) {
			Debug.LogError ("No fMap");
			return false;
		}

		prev_btn_state = data.Btn;
		data = (LocomotionDevice.dataexchange_t)Marshal.PtrToStructure (IPCMapPntr, typeof(LocomotionDevice.dataexchange_t));	

		return true;
	}

	public static Quaternion GetQuaternion()
	{
		Quaternion q_in = new Quaternion (-data.Qr_x, data.Qr_z, data.Qr_y, data.Qr_w); //mapping to Unity coordinate system
		return q_in;
	}

	public static bool GetButton(Btns button)
	{
		return (((data.Btn & (int)button) > 0) && ((prev_btn_state & (int)button) > 0));
	}

	public static bool GetButtonDown(Btns button)
	{
		return (((data.Btn & (int)button) > 0) && ((prev_btn_state & (int)button) == 0));
	}

	public static bool GetButtonUp(Btns button)
	{
		return (((data.Btn & (int)button) == 0) && ((prev_btn_state & (int)button) > 0));
	}
	
	public static bool IsValidSignature(byte SigData)
	{
		return (SigData == COMLINK_SIGNATURE); 	
	}	

}
