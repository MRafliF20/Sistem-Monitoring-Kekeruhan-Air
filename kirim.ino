void kirim() {
  Serial.print("connecting to ");
  Serial.println(host);
//  arus = 2;
//  tegangan = 9;
  data = "air=" + String(kekeruhan);
  WiFiClient client;
  const int httpPort = 80;
  if (client.connect(host, httpPort)) { //Ganti dengan alamat website anda. Utk localhost, ganti dengan 127.0.0.1
    client.print(String("GET ") + url + "?" + data + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n"
                 "Connection: close\r\n\r\n");
                 Serial.println("Koneksi Sukses");
  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }

  delay(2000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}
