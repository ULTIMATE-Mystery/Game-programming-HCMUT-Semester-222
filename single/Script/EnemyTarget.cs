using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyTarget : MonoBehaviour
{
  SphereCollider myCollider;
  EnemyBomb enemyBomb;
  public GameObject enemy;

  void Start()
  {
    myCollider = GetComponent<SphereCollider>();
    enemyBomb = enemy.GetComponent<EnemyBomb>();

    StartCoroutine(Explode());
  }

  private void OnTriggerEnter(Collider other)
  {
    if (other.tag == "Player")
    {
      PlayerHealth playerHealth = other.gameObject.GetComponent<PlayerHealth>();

      // If the EnemyHealth component exist...
      if (playerHealth != null)
      {
        // ... the enemy should take damage.
        playerHealth.TakeDamage(enemyBomb.damagePerShot);
      }
    }

    Destroy(gameObject);
  }

  IEnumerator Explode()
  {
    yield return new WaitForSecondsRealtime(0.5f);
    myCollider.enabled = true;
    Destroy(gameObject, 0.5f);
  }
}
