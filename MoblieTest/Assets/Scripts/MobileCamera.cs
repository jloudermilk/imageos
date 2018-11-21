﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using System.Runtime.InteropServices;

public class MobileCamera : MonoBehaviour
{

    

    bool camAvailable;
    bool front;
    WebCamTexture backCam;
    WebCamTexture frontCam;
    Texture defaultBackground;
    Texture2D snap;

    public RawImage Background;
    public AspectRatioFitter Fitter;
    public WebCamTexture CurrentCam;
    public PaletteObject palette;

    [DllImport("PaletteChanger")]

    private static extern IntPtr SubmitImage(IntPtr bytes);
    [DllImport("PaletteChanger")]
    private static extern bool Startup();


    private void Start()
    {
        defaultBackground = Background.texture;
        WebCamDevice[] devices = WebCamTexture.devices;

        if (devices.Length < 1)
        {
            Debug.LogError("no cameras detected");
            camAvailable = false;
            return;
        }
        foreach (var cam in devices)
        {
            if (cam.isFrontFacing)
            {
                frontCam = new WebCamTexture(cam.name, Screen.width, Screen.height);
            }
            else
            {
                backCam = new WebCamTexture(cam.name, Screen.width, Screen.height);
            }
        }

        if (backCam == null)
        {
            Debug.LogError("no back camera detected");
            camAvailable = false;
            return;
        }

        CurrentCam = backCam;
        CurrentCam.Play();
        Background.texture = CurrentCam;
        camAvailable = true;
        front = false;

       
    }
    private void Update()
    {
        if (!camAvailable)
        {
            return;
        }
        float ratio = (float)CurrentCam.width / (float)CurrentCam.height;
        Fitter.aspectRatio = ratio;

        float scaleY = CurrentCam.videoVerticallyMirrored ? -1f : 1f;
        Background.rectTransform.localScale = new Vector3(1f, scaleY, 1f);

        int orient = -CurrentCam.videoRotationAngle;
        Background.rectTransform.localEulerAngles = new Vector3(0, 0, orient);


    }
    public void SnapPic()
    {
        snap = new Texture2D(CurrentCam.width, CurrentCam.height);
        snap.SetPixels(CurrentCam.GetPixels());
        snap.Apply();
        Startup();
        SubmitImage(snap.GetNativeTexturePtr());
        Background.texture = snap;
        camAvailable = false;
        
    }
    public void SwapCameras()
    {
        if (front)
        {
            CurrentCam = backCam;
            front = false;
        }
        else
        {
            CurrentCam = frontCam;
            front = true;
        }

        CurrentCam.Play();
        Background.texture = CurrentCam;
        camAvailable = true;
    }
}
