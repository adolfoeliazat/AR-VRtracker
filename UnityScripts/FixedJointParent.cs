using UnityEngine;
using System.Collections;

public class FixedJointParent : MonoBehaviour
{

	GameObject center_eye_anchor;
	// Use this for initialization
	void Start ()
	{
		center_eye_anchor = GameObject.Find ("CenterEyeAnchor");
	}
	
	// Update is called once per frame
	void Update ()
	{
		Vector3 pos = center_eye_anchor.transform.position;
		//check for magnitude
		//OVR intgratin sometimes give position vector with inf values
		if (pos.magnitude < 100) {
			pos.y -= 0.1f;
			pos.z -= 0.05f;
			
			transform.position = pos;
			transform.rotation = center_eye_anchor.transform.rotation;
		}

	}
}
