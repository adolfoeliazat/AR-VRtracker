using UnityEngine;
using System.Collections;

public class FixedRotationParent : MonoBehaviour {
	Quaternion rot;

	void Start () {
		rot = transform.rotation;
	}
	
	// Update is called once per frame
	void LateUpdate () {
		transform.rotation = rot;
	}
}
