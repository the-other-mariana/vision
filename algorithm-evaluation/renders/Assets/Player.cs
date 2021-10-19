using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public SnapshotCamera snapCam;

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            snapCam.TakeSnapshot();
        }
    }
}
