#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H
#include "utils/currency/CurrencyUtils.h"
#include "utils/EPPROM/EEPROMUtils.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * {
      box-sizing: border-box;
    }
    html {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f8f9fa;
    }
  html, body {
    max-width: 100%;
    overflow-x: hidden;
    margin: 0;
    padding: 0;
  }
  .container {
    width: 100%;
    max-width: 400px;
    margin: auto;
  }

  .table-container {
    width: 100%;
    overflow-y: auto; /* Allow vertical scroll if needed */
  }
	.hidden {
      display: none;
    }

.message {
  margin-top: 20px;
  margin-bottom: 20px;
  font-size: 1.2rem;
  color: green;
}
  table {
    width: 100%;
    table-layout: fixed; /* Prevent expanding beyond screen */
  }
    h1 {
      font-size: 2.5rem;
      margin-top: 10px;
      color: #333;
    }
    h2 {
      font-size: 1.8rem;
      color: #555;
    }
    .button {
      border: none;
      color: white;
      padding: 14px 0;
      font-size: 18px;
      cursor: pointer;
      border-radius: 6px;
      transition: 0.3s;
      width: 100%;
    }
    .add-button {
      background-color: #4CAF50;
    }
    .submit-button {
      background-color: #007bff;
      font-weight: bold;
      margin-top: 10px;
	  margin-bottom: 20px;
    }
    .button:hover {
      opacity: 0.8;
    }
    .input, .dropdown {
      font-size: 18px;
      padding: 12px;
      width: 100%;
      border: 1px solid #ccc;
      border-radius: 5px;
      margin-bottom: 10px;
    }
    .container {
      width: 90%;
      max-width: 400px;
      margin: auto;
    }
    .card {
      background: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.1);
      margin-top: 20px;
    }
    .table-container {
      overflow-x: auto;
    }
    table {
      width: 100%;
      border-collapse: collapse;
      margin-top: 10px;
    }
    th, td {
      border: 1px solid black;
      padding: 8px;
      text-align: center;
      font-size: 14px;
    }
    th {
      background-color: #f2f2f2;
    }
  </style>
</head>
<body>

  <h1>Clocky</h1>

  <div class="container">
    <div class="card">
      <h2>WiFi Settings</h2>
      <input id="ssid" class="input" type="text" maxlength="32" placeholder="Enter SSID" value="%SSID%">
      <input id="password" class="input" type="password" placeholder="Enter Password" value="%PASSWORD%">
	  <label for="showPassword">
		<input type="checkbox" id="showPassword"> Show Password
	  </label>
    </div>

    <div class="card">
      <h2>Currency Exchange</h2>
      <select id="fromCurrency" class="dropdown">
        <option value="USD">USD - US Dollar</option>
        <option value="EUR">EUR - Euro</option>
        <option value="GBP">GBP - British Pound</option>
        <option value="JPY">JPY - Japanese Yen</option>
        <option value="UAH">UAH - Ukrainian Hryvnia</option>
      </select>

      <select id="toCurrency" class="dropdown">
        <option value="USD">USD - US Dollar</option>
        <option value="EUR">EUR - Euro</option>
        <option value="GBP">GBP - British Pound</option>
        <option value="JPY">JPY - Japanese Yen</option>
        <option value="UAH">UAH - Ukrainian Hryvnia</option>
      </select>

      <button class="button add-button" onclick="addCurrencyPair()">Add Pair</button>
    </div>

    <div class="card table-container">
      <h2>Selected Currency Pairs</h2>
      <table id="pairsTable">
        <thead>
          <tr>
            <th>From</th>
            <th>To</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody id="pairsBody">
        </tbody>
      </table>
    </div>
    <div id="message" class="hidden"></div>
    <button id="submitButton" class="button submit-button" onclick="connectToWifi()">Submit</button>
  </div>

</body>

<script>
  let currencyPairs = %CURRENCYPAIRS_JSON%;

  window.addEventListener('load', () => {
      const tableBody = document.getElementById('pairsBody');
      currencyPairs.forEach(pair => {
        let row = tableBody.insertRow();
        row.insertCell(0).innerText = pair.from;
        row.insertCell(1).innerText = pair.to;
        let deleteCell = row.insertCell(2);
        let deleteButton = document.createElement("button");
        deleteButton.innerText = "Remove";
        deleteButton.onclick = function () {
          // Remove from the currencyPairs array and update table
          const index = currencyPairs.findIndex(p => p.from === pair.from && p.to === pair.to);
          if(index > -1) {
            currencyPairs.splice(index, 1);
            tableBody.deleteRow(row.rowIndex - 1);
          }
        };
        deleteCell.appendChild(deleteButton);
      });
    });

  function addCurrencyPair() {
    if (currencyPairs.length >= 5) {
      alert("You can only add up to 5 currency pairs.");
      return;
    }

    var from = document.getElementById("fromCurrency").value;
    var to = document.getElementById("toCurrency").value;

    if (from === to) {
      alert("From and To currencies should be different.");
      return;
    }

    for (let pair of currencyPairs) {
      if (pair.from === from && pair.to === to) {
        alert("This currency pair is already added.");
        return;
      }
    }

    currencyPairs.push({ from, to });

    let table = document.getElementById("pairsBody");
    let row = table.insertRow();
    row.insertCell(0).innerText = from;
    row.insertCell(1).innerText = to;

    let deleteCell = row.insertCell(2);
    let deleteButton = document.createElement("button");
    deleteButton.innerText = "Remove";
    deleteButton.onclick = function () {
      let index = currencyPairs.findIndex(p => p.from === from && p.to === to);
      if (index > -1) {
        currencyPairs.splice(index, 1);
        table.deleteRow(row.rowIndex - 1);
      }
    };
    deleteCell.appendChild(deleteButton);
  }

  function connectToWifi() {
    var ssid = document.getElementById("ssid").value;
    var password = document.getElementById("password").value;

    if (!ssid.trim()) {
      alert("SSID cannot be empty.");
      return;
    }

	var submitButton = document.getElementById("submitButton");
	submitButton.disabled = true;
	submitButton.innerText = "Submitting...";

    var data = {
      ssid: ssid,
      password: password,
      currencyPairs: currencyPairs
    };

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/", true);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.send(JSON.stringify(data));

	xhr.onload = function () {
    if (xhr.status === 200) {
      document.getElementById("message").classList.remove("hidden");
      document.getElementById("message").innerText = "Settings saved successfully. Please restart the device.";
	  submitButton.innerText = "Submitted";
    } else {
      document.getElementById("message").classList.remove("hidden");
      document.getElementById("message").innerText = "Error saving settings. Please try again.";
	  submitButton.innerText = "Failed";
    }
  };
  }

  document.getElementById("showPassword").addEventListener("change", function() {
  var passwordField = document.getElementById("password");
  if (this.checked) {
    passwordField.type = "text"; // Show password
  } else {
    passwordField.type = "password"; // Hide password
  }
});
</script>

</html>
)rawliteral";

inline String getHTMLPage(bool addPrefilledFields) {
    String ssid = "";
    String password = "";
    String pairsJson = "[]";

    if (addPrefilledFields) {
        Pair<String, String> wifiData = readWifiDataFromEEPROM();
        ssid = wifiData.key.c_str();
        password = wifiData.value.c_str();
        std::vector<Pair<Currency, Currency> > storedPairs = readCurrencyPairs();

        pairsJson = "[";
        for (size_t i = 0; i < storedPairs.size(); i++) {
            if (i > 0) pairsJson += ",";

            pairsJson += String("{\"from\":\"")
                    + CurrencyUtils::toString(storedPairs[i].key)
                    + "\",\"to\":\""
                    + CurrencyUtils::toString(storedPairs[i].value)
                    + "\"}";
        }
        pairsJson += "]";
    }

    String htmlPage = FPSTR(index_html);

    htmlPage.replace("%SSID%", ssid);
    htmlPage.replace("%PASSWORD%", password);
    htmlPage.replace("%CURRENCYPAIRS_JSON%", pairsJson);

    return htmlPage;
}

#endif
