using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Claw : MonoBehaviour
{
  // Start is called before the first frame update
  IEnumerator Disappear()
  {
    yield return new WaitForSecondsRealtime(1);
    gameObject.SetActive(false);
  }

  void Update()
  {
    StartCoroutine(Disappear());
  }
}
