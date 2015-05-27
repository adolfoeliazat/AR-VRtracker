using UnityEngine;
using System.Collections;

public class StopTrigger : MonoBehaviour {

	MotionController motionController;
	
	// Use this for initialization
	void Start () {
		motionController = GetComponentInParent<MotionController>();
	}
	
	void OnTriggerEnter(Collider other) {
		motionController.MovementStop ();
	}
}
