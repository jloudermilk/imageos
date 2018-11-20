using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "Palette", menuName = "LEGO/Palette", order = 1)]
public class PaletteObject : ScriptableObject
{
    public string objectName = "New Palette";
    [SerializeField]
    public Color[] colors;

}
