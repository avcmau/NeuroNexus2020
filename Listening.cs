using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Listening : MonoBehaviour
{
    /// <summary>
    /// This code uses open source Arduino-Unity communication protocol: https://ardity.dwilches.com/
    /// Pong game as per tutorial: https://www.awesomeinc.org/tutorials/unity-pong/
    /// </summary>
    private GameObject paddle;
    public float speed = 10.0f;
    public float boundY = 2.25f;
    private Rigidbody2D rb2d;

    // Start is called before the first frame update
    void Start()
    {
        paddle = GameObject.Find("Paddle1");
        rb2d = paddle.GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // Invoked when a line of data is received from the serial device.
    void OnMessageArrived(string msg)
    {
        var data = msg.Split(',');
        float x = float.Parse(data[1]);
        float z = float.Parse(data[0]);

        var vel = rb2d.velocity;
        if (z > 90)
        {
            vel.y = speed;
        }
        else if (z < 90)
        {
            vel.y = -speed;
        }
        else
        {
            vel.y = 0;
        }
        rb2d.velocity = vel;

        var pos = paddle.transform.position;
        if (pos.y > boundY)
        {
            pos.y = boundY;
        }
        else if (pos.y < -boundY)
        {
            pos.y = -boundY;
        }
        paddle.transform.position = pos;
        //UnityEngine.Debug.Log(msg);
    }

    // Invoked when a connect/disconnect event occurs. The parameter 'success'
    // will be 'true' upon connection, and 'false' upon disconnection or
    // failure to connect.
    void OnConnectionEvent(bool success)
    {
        if (!success)
            UnityEngine.Debug.Log("Connection Failed");
    }
}
