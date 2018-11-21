using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.InteropServices;

public class PaletteSwap : MonoBehaviour {

    [DllImport("PaletteChanger")]
    private static extern bool StartUp();
    [DllImport("PaletteChanger")]
    private static extern void AddPalette(float[] hsv, int size);
    [DllImport("PaletteChanger")]
    private static extern IntPtr SubmitImage(IntPtr bytes);


    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
