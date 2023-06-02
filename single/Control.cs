using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Control : MonoBehaviour
{
  [SerializeField] float runSpeed = 10f;
  [SerializeField] GameObject bullet;

  Vector2 moveInput;
  Rigidbody myRigidbody;
  public bool isTarget = false;

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

  void OnMovement(InputValue value)
  {
    if (!isAlive) { return; }
    moveInput = value.Get<Vector2>();
  }

  void OnFire(InputValue value)
  {
    if (!isAlive || isTarget) { return; }
    Vector3 m = Input.mousePosition;
    m.z = Camera.main.nearClipPlane + 25;
    Vector3 a = Camera.main.ScreenToWorldPoint(m);
    a.y = transform.position.y;
    Instantiate(bullet, a, Quaternion.Euler(90, 0, 0));
    isTarget = true;
  }

  void Run()
  {
    Vector3 playerVelocity = new Vector3(moveInput.x * runSpeed, myRigidbody.velocity.y, moveInput.y * runSpeed);
    myRigidbody.velocity = playerVelocity;
  }
}
