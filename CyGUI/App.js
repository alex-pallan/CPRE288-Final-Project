import { useState, useEffect } from "react";
import "./App.css";

function calculateCoordinates(angle) {
  // Convert angle from degrees to radians
  const radians = (angle * Math.PI) / 180;

  // Calculate x and y coordinates using trigonometry
  const x = Math.cos(radians) * 400 + 400;
  const y = Math.sin(radians) * -400 + 400;

  return { x, y };
}

function calculateCoordinatesWithDistance(degrees, distance) {
  // Convert degrees to radians
  var radians = degrees * (Math.PI / 180);

  // Calculate the x and y coordinates
  var x = 400 + distance * 4 * Math.cos(radians);
  var y = 400 - distance * 4 * Math.sin(radians);

  console.log("calculateCoordinatesWithDistance() called!");
  console.log("degrees: ", degrees, " distance: ", distance);
  console.log("Found x: ", x, " y: ", y);

  return { x: x, y: y };
}

function rotatePoint(point, angle) {
  const radians = (angle * Math.PI) / 180;
  const cosTheta = Math.cos(radians);
  const sinTheta = Math.sin(radians);

  const translatedX = point.x - 400;
  const translatedY = point.y - 400;

  const rotatedX = translatedX * cosTheta - translatedY * sinTheta;
  const rotatedY = translatedX * sinTheta + translatedY * cosTheta;

  return {
    x:
      (point.x - 400) * Math.cos((angle * Math.PI) / 180) -
      (point.y - 400) * Math.sin((angle * Math.PI) / 180) +
      400,
    y:
      (point.x - 400) * Math.sin((angle * Math.PI) / 180) +
      (point.y - 400) * Math.cos((angle * Math.PI) / 180) +
      400,
  };
}

function App() {
  const [message, setMessage] = useState("");
  const [IP, setIP] = useState("localhost");
  const [port, setPort] = useState("8080");
  const [receivedMessage, setReceivedMessage] = useState("");
  const [connected, setConnected] = useState(false);
  const [ws, setWs] = useState(null); // State to hold WebSocket connection object
  const [history, setHistory] = useState([]);
  const [scanData, setScanData] = useState([]); // State array to hold all points from scandata.
  const [degree, setDegree] = useState(0); // The current degree of the scan
  const [ping, setPing] = useState(0); // The distance of the ping
  const [IR, setIR] = useState(0); // The distance of the IR
  const [inputValue, setInputValue] = useState(""); // Manual command input value

  const handleChange = (event) => {
    setInputValue(event.target.value);
  };

  const handleClick = () => {
    setMessage(inputValue);
  };

  const sendDriveMessage = () => {
    // Retrieve input and select field values
    const distance = document.getElementById("driveInput").value;
    const direction = document.getElementById("driveDirection").value.charAt(0); // Get the first letter of the selected direction

    // Format the message
    const formattedMessage = `${direction}${distance.padStart(3, "0")}`; // Pad the distance with leading zeros if needed

    // Set the formatted message
    setMessage(formattedMessage);

    // Call sendMessage immediately after setting the message
  };

  const sendRotateMessage = () => {
    // Retrieve input and select field values
    const angle = document.getElementById("rotateInput").value;
    const direction = document
      .getElementById("rotateDirection")
      .value.charAt(0); // Get the first letter of the selected direction

    // Format the message
    const formattedMessage = `${direction}${angle.padStart(3, "0")}`; // Pad the distance with leading zeros if needed

    // Set the formatted message
    setMessage(formattedMessage);

    // Call sendMessage immediately after setting the message
  };

  const sendScanMessage = () => {
    // First set the left scan boundary
    // Format the message
    const leftBoundary = scanPreview.left;
    const rightBoundary = scanPreview.right;
    const formattedLeftMessage = `j${String(leftBoundary).padStart(3, "0")}`; // Pad the distance with leading zeros if needed
    // Format the message for right boundary
    const formattedRightMessage = `k${String(rightBoundary).padStart(3, "0")}`; // Pad the distance with leading zeros if needed
    // Finally, send the scan message along with the pulse width
    const pulseWidth = document.getElementById("pulseWidth").value;
    const formattedPulseWidth = `s${pulseWidth.padStart(3, "0")}`; // Pad the distance with leading zeros if needed

    // Combine all formatted messages
    const combinedMessage =
      formattedLeftMessage + formattedRightMessage + formattedPulseWidth;

    // Set the combined message with delay
    setTimeout(() => {
      setMessage(combinedMessage);
      // Call sendMessage immediately after setting the message
    }, 100); // Delay of 100ms
  };

  const translateScanDataY = (cm) => {
    setScanData((prevScanData) =>
      prevScanData.map((point) => ({
        ...point,
        y: point.y + cm * 4, // Update only the y-coordinate
      }))
    );
  };

  const handleRotate = (angle) => {
    setScanData((prevScanData) =>
      prevScanData.map((point) => ({
        ...point,
        x:
          (point.x - 400) * Math.cos((angle * Math.PI) / 180) -
          (point.y - 400) * Math.sin((angle * Math.PI) / 180) +
          400, // Update the x-coordinate
        y:
          (point.x - 400) * Math.sin((angle * Math.PI) / 180) +
          (point.y - 400) * Math.cos((angle * Math.PI) / 180) +
          400,
      }))
    );
  };

  let newDegree = 0;

  const handleDegree = (event) => {
    newDegree = parseInt(event); // Assign value to newDegree
    setDegree(newDegree);
    console.log("Set the degree!");
    console.log(newDegree);
  };

  const handlePing = (event) => {
    const newPing = parseInt(event);
    setPing(newPing);
    console.log("Received new ping value!");
    const pingCoordinates = calculateCoordinatesWithDistance(
      newDegree, // Use the updated degree value here
      newPing
    );
    setScanData((prevScanData) => [
      ...prevScanData,
      { type: "ping", ...pingCoordinates },
    ]);
  };

  const handleIR = (event) => {
    const newIR = parseInt(event);
    setIR(newIR);
    console.log("Received new IR value!");
    const irCoordinates = calculateCoordinatesWithDistance(
      newDegree, // Use the updated degree value here
      newIR
    );
    setScanData((prevScanData) => [
      ...prevScanData,
      { type: "IR", ...irCoordinates },
    ]);
  };

  const handleHole = (event) => {
    const newHole = parseInt(event);
    console.log("Received new hole value!");
    const holeCoordinates = calculateCoordinatesWithDistance(
      90, // Use the updated degree value here
      newHole
    );
    setScanData((prevScanData) => [
      ...prevScanData,
      { type: "hole", ...holeCoordinates },
    ]);
  };

  const handleBounds = (event) => {
    const newBound = parseInt(event);
    console.log("Received new bounds value!");
    const boundCoordinates = calculateCoordinatesWithDistance(
      90, // Use the updated degree value here
      newBound
    );
    setScanData((prevScanData) => [
      ...prevScanData,
      { type: "bound", ...boundCoordinates },
    ]);
  };

  const handleBump = (event) => {
    const newBump = parseInt(event);
    console.log("Received new bump value!");
    const bumpCoordinates = calculateCoordinatesWithDistance(
      90, // Use the updated degree value here
      newBump
    );
    setScanData((prevScanData) => [
      ...prevScanData,
      { type: "bump", ...bumpCoordinates },
    ]);
  };

  useEffect(() => {
    if (connected) {
      const newWs = new WebSocket(`ws://${IP}:${port}`);

      newWs.onopen = () => {
        console.log("WebSocket connected");
      };

      let newDegree; // Declare newDegree here

      newWs.onmessage = (event) => {
        console.log("Received message:", event.data);
        history.push({ endpoint: "ROBOT", message: event.data });
        setReceivedMessage(event.data);

        // Split the message into individual lines
        const lines = event.data.split("\n");

        // Iterate over each line
        lines.forEach((line) => {
          // Trim the line to remove any leading or trailing whitespace
          const trimmedLine = line.trim();

          // Check if the line is not empty
          if (trimmedLine.length > 0) {
            // Process each line based on its first character
            switch (trimmedLine.charAt(0)) {
              case "d":
                handleDegree(trimmedLine.substring(1));
                break;
              case "p":
                handlePing(trimmedLine.substring(1));
                break;
              case "i":
                handleIR(trimmedLine.substring(1));
                break;
              case "l":
                handleRotate(parseInt(trimmedLine.substring(1)));
                break;
              case "r":
                handleRotate(-1 * parseInt(trimmedLine.substring(1)));
                break;
              case "f":
                translateScanDataY(parseInt(trimmedLine.substring(1)));
                console.log("Moved forward ", trimmedLine.substring(1));
                break;
              case "b":
                translateScanDataY(-1 * parseInt(trimmedLine.substring(1)));
                console.log("Moved backward ", trimmedLine.substring(1));
                break;
              case "h":
                handleHole(trimmedLine.substring(2)); // Add a hole to the scanData for visualization
                translateScanDataY(parseInt(trimmedLine.substring(2))); // Move the data back
              case "w":
                handleBounds(trimmedLine.substring(2));
              case "q":
                handleBump(trimmedLine.substring(1));
              case "e":
                handleBump(trimmedLine.substring(1));
              default:
                console.log("Unknown command:", trimmedLine);
                break;
            }
          }
        });
      };

      newWs.onclose = () => {
        console.log("WebSocket disconnected");
        setConnected(false);
      };

      setWs(newWs); // Update WebSocket connection object
    } else {
      if (ws) {
        ws.close();
      }
    }

    // Cleanup function
    return () => {
      if (ws) {
        ws.close();
      }
    };
  }, [connected]);

  const toggleConnection = () => {
    if (connected) {
      disconnectFromServer();
    } else {
      connectToServer();
    }
  };

  const connectToServer = () => {
    setConnected(true);
  };

  const disconnectFromServer = () => {
    setConnected(false);
  };

  const sendMessage = () => {
    if (ws && ws.readyState === WebSocket.OPEN && message.trim() !== "") {
      console.log("Sending message:", message);
      history.push({ endpoint: "CLIENT", message: message });
      ws.send(message);
      setMessage("");
    }
  };

  const [scanPreview, setScanPreview] = useState({ left: 180, right: 0 });
  const [lineCoordinates, setLineCoordinates] = useState({
    leftX2: 400,
    leftY2: 400,
    rightX2: 800,
    rightY2: 400,
  });

  const handleScanChange = (e, field) => {
    const newValue = parseInt(e.target.value);
    setScanPreview({ ...scanPreview, [field]: newValue });

    const { x, y } = calculateCoordinates(newValue);

    if (field === "left") {
      setLineCoordinates((prevState) => ({
        ...prevState,
        leftX2: x,
        leftY2: y,
      }));
    } else if (field === "right") {
      setLineCoordinates((prevState) => ({
        ...prevState,
        rightX2: x,
        rightY2: y,
      }));
    }
  };

  useEffect(() => {
    if (message.trim() !== "") {
      sendMessage();
    }
  }, [message, sendMessage]);

  return (
    <>
      <div className="header">
        <h1>CyGUI</h1>
      </div>
      <div className="container">
        <div className="radar-container">
          <svg
            width="800"
            height="400"
            style={{
              borderRadius: "10px",
              borderBottomLeftRadius: "0",
              borderBottomRightRadius: "0",
              border: "1px solid darkslategray",
              background: "black",
            }}
          >
            <circle cx="50%" cy="100%" r="400" fill="#033E3E">
              <title>Background</title>
            </circle>
            <g>
              <circle
                cx="50%"
                cy="100%"
                r="400"
                fill="transparent"
                stroke="darkgreen"
                strokeWidth="1"
              >
                <title>75cm-100cm</title>
              </circle>
              <text
                x="0%"
                y="100%"
                fill="darkgreen"
                font-size="10"
                fontFamily="sans-serif"
              >
                100cm
              </text>
              <circle
                cx="50%"
                cy="100%"
                r="300"
                fill="transparent"
                stroke="darkgreen"
                strokeWidth="1"
              >
                <title>50cm-75cm</title>
              </circle>
              <text
                x="12.5%"
                y="100%"
                fill="darkgreen"
                font-size="10"
                fontFamily="sans-serif"
              >
                75cm
              </text>
              <circle
                cx="50%"
                cy="100%"
                r="200"
                fill="transparent"
                stroke="darkgreen"
                strokeWidth="1"
              >
                <title>25cm-50cm</title>
              </circle>
              <text
                x="25%"
                y="100%"
                fill="darkgreen"
                font-size="10"
                fontFamily="sans-serif"
              >
                50cm
              </text>
              <circle
                cx="50%"
                cy="100%"
                r="100"
                fill="transparent"
                stroke="darkgreen"
                strokeWidth="1"
              >
                <title>0cm-25cm</title>
              </circle>
              <text
                x="37.5%"
                y="100%"
                fill="darkgreen"
                font-size="10"
                fontFamily="sans-serif"
              >
                25cm
              </text>
            </g>
            <g>
              <line
                x1="400"
                y1="400"
                x2="400"
                y2="0"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>90deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="746.41"
                y2="200"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>30deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="53.59"
                y2="200"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>150deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="600"
                y2="53.59"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>60deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="200"
                y2="53.59"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>120deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="682.843"
                y2="117.157"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>45deg</title>
              </line>
              <line
                x1="400"
                y1="400"
                x2="117.157"
                y2="117.157"
                style={{ stroke: "darkgreen", strokeWidth: "1" }}
              >
                <title>135deg</title>
              </line>
            </g>
            <line
              x1="400"
              y1="400"
              x2={lineCoordinates.leftX2}
              y2={lineCoordinates.leftY2}
              style={{ stroke: "white", strokeWidth: "2" }}
            ></line>
            <line
              x1="400"
              y1="400"
              x2={lineCoordinates.rightX2}
              y2={lineCoordinates.rightY2}
              style={{ stroke: "white", strokeWidth: "2" }}
            ></line>

            <line
              x1="400"
              y1="400"
              x2={calculateCoordinates(degree).x}
              y2={calculateCoordinates(degree).y}
              style={{
                stroke: "green",
                strokeWidth: "2",
                strokeOpacity: "0.5",
              }}
            ></line>

            {scanData.map((data, index) => (
              <circle
                key={index}
                cx={data.x}
                cy={data.y}
                r="3"
                fill={
                  data.type === "ping"
                    ? "lightblue"
                    : data.type === "IR"
                    ? "red"
                    : data.type === "hole"
                    ? "lightgray"
                    : data.type === "bounds"
                    ? "white"
                    : data.type === "bump"
                    ? "orange"
                    : "yellow" // Default color if type is unknown
                }
              />
            ))}

            <circle cx="50%" cy="460" r="70" fill="gray">
              <title>CyBot</title>
            </circle>
          </svg>
          <div className="control-panel">
            <p>Control Panel</p>
            <div className="grid-container">
              <div className="controls-container">
                <details>
                  <summary>Rotation</summary>
                  <hr />
                  <div className="controls">
                    Rotate{" "}
                    <input
                      type="text"
                      id="rotateInput"
                      style={{ width: "25px" }}
                      disabled={!connected}
                    />{" "}
                    degrees to the{" "}
                    <select id="rotateDirection" disabled={!connected}>
                      <option value="left">left</option>
                      <option value="right">right</option>
                    </select>
                    .<br />
                    <br />
                    <button onClick={sendRotateMessage} disabled={!connected}>
                      Send Command
                    </button>
                  </div>
                </details>
              </div>
              <div className="controls-container">
                <details>
                  <summary>Drive</summary>
                  <hr />
                  <div className="controls">
                    Drive{" "}
                    <input
                      type="text"
                      id="driveInput"
                      style={{ width: "25px" }}
                      disabled={!connected}
                    />
                    cm&nbsp;
                    <select id="driveDirection" disabled={!connected}>
                      <option value="forward">forward</option>
                      <option value="backward">backwards</option>
                    </select>
                    .
                    <br />
                    <br />
                    <button onClick={sendDriveMessage} disabled={!connected}>
                      Send Command
                    </button>
                  </div>
                </details>
              </div>
              <div className="controls-container">
                <details>
                  <summary>Scan</summary>
                  <hr />
                  <div className="controls">
                    Scan Parameters:
                    <br />
                    From{" "}
                    <input
                      type="number"
                      value={scanPreview.left}
                      onChange={(e) => handleScanChange(e, "left")}
                      style={{ width: "40px" }}
                      min={1}
                      max={180}
                      disabled={!connected}
                    />
                    deg to{" "}
                    <input
                      type="number"
                      value={scanPreview.right}
                      onChange={(e) => handleScanChange(e, "right")}
                      style={{ width: "40px" }}
                      min={0}
                      max={179}
                      disabled={!connected}
                    />
                    deg.
                    <br />
                    Pulse every{" "}
                    <input
                      type="text"
                      style={{ width: "25px" }}
                      id="pulseWidth"
                      disabled={!connected}
                      defaultValue="2"
                    />
                    deg.
                    <br />
                    <br />
                    <button disabled={!connected} onClick={sendScanMessage}>
                      Send Command
                    </button>
                  </div>
                </details>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div
        style={{
          position: "absolute",
          top: "0",
          left: "0",
          margin: "10px",
          display: "flex",
          alignItems: "center",
        }}
      >
        <svg width="15" height="15">
          <circle
            cx="7.5"
            cy="7.5"
            r="7.5"
            stroke="black"
            strokeWidth={1}
            fill={connected ? "green" : "red"}
          />
        </svg>
        <button
          style={{ margin: "0", marginLeft: "10px" }}
          onClick={toggleConnection}
        >
          {connected ? "Disconnect" : "Connect"}
        </button>{" "}
      </div>
      <details class="command-container-dropdown" open={!connected}>
        <summary>Manual Commands</summary>
        <div class="command-container">
          <div class="previous-commands">
            <ul>
              {history.map((entry, index) => (
                <li key={index}>
                  {entry.endpoint}: {entry.message}
                </li>
              ))}
            </ul>
          </div>
          <div class="command-entry-container">
            <input
              type="text"
              id="commandInput"
              value={inputValue}
              onChange={handleChange}
              disabled={!connected}
            />
            <button onClick={handleClick} disabled={!connected}>
              Send
            </button>
            <details open={!connected}>
              <summary>
                <u>Connection Config</u>
              </summary>
              IP Address:
              <br />
              <input
                type="text"
                value={IP}
                onChange={(e) => setIP(e.target.value)}
                disabled={connected}
              />
              <br />
              Port
              <br />
              <input
                type="text"
                value={port}
                onChange={(e) => setPort(e.target.value)}
                disabled={connected}
              />
            </details>
          </div>
        </div>
      </details>
    </>
  );
}

export default App;
