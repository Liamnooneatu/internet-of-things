String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en" > <!-- Needed for Screenreaders !-->
<head>
<!-- UTF-8 character set covers most characters in the world -->
  <meta charset="utf-8">
  <!-- Make page respond to screen size !-->
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <meta http-equiv="refresh" content="30">
<!--Include a Title. Used by Search Engines -->
<title>Temperature Reading WebServer</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css">
<style>
  body {       
    background-color: LightGreen;
    font-family: 'Roboto', sans-serif;
  }
  .flex-Container {
    display: flex;
    flex-direction: column;
    background-color: LightGreen;
    align-items: center;
  }
  h1 {
    font: bold;
    font-size: 40px;
    font-family: 'Roboto', sans-serif;
    color: navy;
    text-align: center;
  }
  p {
    font-size: 25px;
    font-family: 'Roboto', sans-serif;
    color: navy;
    text-align: center;
  }  
  th, td {
    font-size: 25px;
    padding: 8px;
    text-align: left;
    border-bottom: 8px solid #ddd;
    font-weight: bold;
    font-family: 'Roboto', sans-serif;
  }
  .toggle-box {
    display: none;
    width: 100%;
    padding: 10px;
    border: 2px solid navy;
    background-color: LightBlue;
    margin-top: 10px;
  }
  .social-icons {
    font-size: 24px;
    margin-top: 10px;
    text-align: center; /* Centering the icons */
  }
  .contact-details {
    margin-top: 20px;
    padding: 20px;
    border: 2px solid navy;
    background-color: LightBlue;
    text-align: center;
    width: 80%;
  }
</style>
</head>
	<body>
		<div class="flex-Container">
		<h1> Temperature Example Website </h1>
     <h2> My ThingSpeak Chart</h2>
    <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/225409/charts/4?bgcolor=%23ffffff&color=%23d62020&results=60&title=Temperature&xaxis=time&yaxis=Temp+C"></iframe>   
      
		<p>Welcome to my website displaying the temperature in ATU</p>
    
		<table>
        <tr>
          <th>Sensor</th>
          <th>Value</th>
          <th>Unit</th>
        </tr>
        <tr>
          <td>Temperature</td>
          <td>  )=====");
String homePagePart2 = F(R"=====(</td>
         <td>Degree Cel</td>
        </tr>
		<tr>
          <td>Relative Humidity</td>
          <td> )=====");
String homePagePart3 = F(R"=====(</td>
         <td>%</td>
        </tr>
		<tr>
          <td>Heart Rate</td>
          <td> )=====");

String homePagePart4 = F(R"=====(</td>
         <td>Beats per minute</td>
        </tr>
		<tr>
          <td>Body temp</td>
          <td> )=====");

String homePagePart5 = F(R"=====(</td>
         <td>Degrees Celsius°C</td>
        </tr>
      </table>
     </div>	

 <p>Welcome to my website displaying the temperature in ATU</p>
  <button onclick="toggleInfo('temp')">Room Temperature</button>
  <div id="temp-box" class="toggle-box">
    <p>This is the temperature information box which is measured via a DHT11 using a thermistor.</p>
  </div>
  <button onclick="toggleInfo('humidity')">Relative Humidity</button>
  <div id="humidity-box" class="toggle-box">
    <p>This is the relative humidity information box which measures the humidity by using a DHT11 that measures water vapor in the air.</p>
  </div>
  <button onclick="toggleInfo('heartRate')">Heart Rate</button>
  <div id="heartRate-box" class="toggle-box">
    <p>This is the heart rate information box. It measures the patient's heart rate using a MAX30100 that measures changes in blood volume under the patient's peripheral tissues.</p>
  </div>
  <button onclick="toggleInfo('bodyTemp')">Body Temperature</button>
  <div id="bodyTemp-box" class="toggle-box">
    <p>This is the body temperature information box. It is measured using an LM35, which a patient presses their finger against to measure their body heat.</p>
  </div>



  <div class="social-icons">
    <i class="fab fa-facebook-official"></i>
    <i class="fab fa-instagram"></i>
    <i class="fab fa-snapchat"></i>
    <i class="fab fa-pinterest"></i>
  </div>
  
  <div class="contact-details">
    <p>Contact Details:</p>
    <p>Name: Liam</p>
    <p>Email: liamnoone2593@gmail.com</p>
    <p>Phone Number: 089524578</p>
  </div>


  <script>
function toggleInfo(id) {
  var infoBox = document.getElementById(id + '-box');
  infoBox.style.display = infoBox.style.display === 'none' ? 'block' : 'none';
  if (infoBox.style.display === 'block') {
    // Fill the box with information
    var info = "";
    switch (id) {
      case 'temp':
        info = "Current temperature: 25°C";
        break;
      case 'humidity':
        info = "Relative humidity: 60%";
        break;
      case 'heartRate':
        info = "Heart rate: 80 bpm";
        break;
      case 'bodyTemp':
        info = "Body temperature: 37°C";
        break;
    }
    document.getElementById(id + '-info').textContent = info;
  }
}
</script>


  </body>
</html>)=====");
