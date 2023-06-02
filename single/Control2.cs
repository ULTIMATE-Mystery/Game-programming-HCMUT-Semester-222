using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Control2 : MonoBehaviour
{
  [SerializeField] float runSpeed = 10f;
  [SerializeField] GameObject claw;

  Vector2 moveInput;
  Rigidbody myRigidbody;
  //public bool isTarget = false;

  bool isAlive = true;


  void Start()
  {
    myRigidbody = GetComponent<Rigidbody>();
  }

  void Update()
  {
    if (!isAlive) { return; }
    Run();
  }

  void OnMove(InputValue value)
  {
    if (!isAlive) { return; }
    moveInput = value.Get<Vector2>();
  }

  void OnFire(InputValue value)
  {
    if (!isAlive) { return; }
    claw.SetActive(true);
    //isTarget = true;
  }

  void Run()
  {
    Vector3 playerVelocity = new Vector3(moveInput.x * runSpeed, myRigidbody.velocity.y, moveInput.y * runSpeed);
    myRigidbody.velocity = playerVelocity;
  }
}
