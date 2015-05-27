using UnityEngine;
using System.Diagnostics;
using System.Collections;

public class MotionController : MonoBehaviour
{
	
	Quaternion dev_orient_q, initial_q, result_q;
	Vector3 old_pos, cur_pos, velocity;
	bool isTriggered, isEnabled;
	Vector3 movementDirection;
	public float speed = 0.5f;

	GameObject ovr_camera, center_eye_anchor, triggers, p_origin, neck_link, waist_link;

	// Use this for initialization
	void Start ()
	{
		isTriggered = false;
		isEnabled = false;

		UnityEngine.Debug.LogError ("Start");
		LocomotionDevice.ConnenctToMemFile ();
		
		LocomotionDevice.ReadHidDevice ();
		initial_q = LocomotionDevice.GetQuaternion ();

		old_pos = transform.position;

		ovr_camera = GameObject.Find ("OVRCameraRig");
		neck_link = GameObject.Find ("NeckLink");
		waist_link = GameObject.Find ("WaistLink");


		triggers = transform.GetChild (1).gameObject; //dir_triggers is second child of motion_controller
		p_origin = transform.GetChild (0).gameObject; //origin is first child
		cur_pos.Set (waist_link.transform.position.x, transform.position.y, waist_link.transform.position.z);
		triggers.transform.position = cur_pos;

	}
	
	void OnDisable ()
	{
		if (gameObject.activeSelf) {
			UnityEngine.Debug.LogError ("Script was disabled");
		} else {
			UnityEngine.Debug.LogError ("Object was destroyed or made inactive");
			LocomotionDevice.DisconnecFromMemFile ();
		}
	}

	void Update ()
	{
		LocomotionDevice.ReadHidDevice ();

		if (LocomotionDevice.GetButtonDown (LocomotionDevice.Btns.Front)) {
			UnityEngine.Debug.LogError ("Front button pressed!");

			OVRManager.display.RecenterPose ();
			//move up for standing
			Vector3 standing_pose = ovr_camera.transform.position;
			standing_pose.y = 1.67f;
			ovr_camera.transform.position = standing_pose;

			initial_q = LocomotionDevice.GetQuaternion ();
		}
		
		if (LocomotionDevice.GetButtonDown (LocomotionDevice.Btns.Top)) {
			UnityEngine.Debug.LogError ("Top button pressed!");
			cur_pos.Set (waist_link.transform.position.x, transform.position.y, waist_link.transform.position.z);
			triggers.transform.position = cur_pos;
		}

		if (LocomotionDevice.GetButtonDown (LocomotionDevice.Btns.Side)) {
			UnityEngine.Debug.LogError ("Side button pressed!");
			if (isEnabled) {
				isEnabled = false;
				//show camera cone
			} else {
				isEnabled = true;
				//hide camera cone
			}
		}
		
		dev_orient_q = LocomotionDevice.GetQuaternion ();
		result_q = dev_orient_q * Quaternion.Inverse (initial_q);
		neck_link.transform.rotation = result_q;

		result_q.x = 0.0f;
		result_q.z = 0.0f;
		transform.rotation = result_q;

		Vector3 pos = new Vector3 (waist_link.transform.position.x, transform.position.y, waist_link.transform.position.z);
		p_origin.transform.position = pos;

		//speed calculation
		cur_pos = pos;
		velocity = (cur_pos - old_pos) / Time.deltaTime;
		old_pos = cur_pos;

		if (isTriggered && isEnabled) {
			transform.Translate (movementDirection * speed * Time.deltaTime);
		}
	}

	public void MovementStart (Vector3 direction)
	{
		if (!isTriggered) {
			isTriggered = true;
			movementDirection = direction;
			speed = velocity.magnitude;
			UnityEngine.Debug.LogError (speed);
			UnityEngine.Debug.LogError ("Movement started");
		}
	}

	public void MovementStop ()
	{
		isTriggered = false;
		UnityEngine.Debug.LogError ("Movement stopped");
	}

}
