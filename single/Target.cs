using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Target : MonoBehaviour
{
  Vector3 screen;
  Vector3 world;
  bool isLanded = false;
  Control tankControl;
  // Start is called before the first frame update
  void Awake()
  {
    tankControl = FindObjectOfType<Control>();
  }
  // Update is called once per frame
  void Update()
  {
    if (!isLanded)
    {
      screen = Input.mousePosition;
      screen.z = Camera.main.nearClipPlane + 25;

      world = Camera.main.ScreenToWorldPoint(screen);
      world.y = 1;
      transform.position = world;
    }
    if (Input.GetMouseButtonDown(0))
    {
      isLanded = true;
      StartCoroutine(Explode());
    }
  }

  IEnumerator Explode()
  {
    yield return new WaitForSecondsRealtime(1);
    Destroy(gameObject);
    tankControl.isTarget = false;
  }
}
