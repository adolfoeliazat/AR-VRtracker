using UnityEngine;
using System.Collections;

public class DirectionTrigger : MonoBehaviour {

	MotionController motionController;

	void Start () {
		motionController = GetComponentInParent<MotionController>();
	}
	
	void OnTriggerEnter(Collider other) {
		motionController.MovementStart(transform.localRotation * Vector3.forward);
	}
}
