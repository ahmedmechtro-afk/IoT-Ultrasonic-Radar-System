float distance = 0;
float angle = 0;

String readApiKey = "ZMMUCZRML1NF0XQU"; // ThingSpeak Read API Key
String channelID = "3195340";           // Channel ID
String url = "https://api.thingspeak.com/channels/" + channelID +
             "/feeds.json?api_key=" + readApiKey + "&results=1";

ArrayList<PVector> blips = new ArrayList<PVector>();
ArrayList<Integer> alphas = new ArrayList<Integer>();
int maxTrail = 50;

boolean sweepForward = true; // For back-and-forth sweep
float sweepAngle = 0;        // Current sweep angle in degrees
float sweepSpeed = 1;        // Degrees per frame

int fetchInterval = 5000;
int lastFetch = 0;

void setup() {
  size(600, 600);
  background(0);
  smooth();
  frameRate(60);
}

void draw() {
  background(0, 20);
  translate(width/2, height/2);

  drawRadarGrid();

  // --- Back-and-forth sweep line ---
  if (sweepForward) {
    sweepAngle += sweepSpeed;
    if (sweepAngle >= 180) sweepForward = false;
  } else {
    sweepAngle -= sweepSpeed;
    if (sweepAngle <= 0) sweepForward = true;
  }

  float sweepRad = radians(sweepAngle);
  stroke(0, 255, 0, 150);
  line(0, 0,
       250 * cos(sweepRad - PI/2),
       250 * sin(sweepRad - PI/2));

  // --- Fetch data from ThingSpeak ---
  if (millis() - lastFetch > fetchInterval) {
    lastFetch = millis();
    fetchData();

    // 🔴 SHOW RED DOT ONLY WHEN DISTANCE ≤ 30 CM
    if (distance <= 30 && distance > 0) {

      float radarDist = map(distance, 0, 200, 0, 250);
      float rad = radians(angle);
      float x = radarDist * cos(rad - PI/2);
      float y = radarDist * sin(rad - PI/2);

      blips.add(new PVector(x, y));
      alphas.add(255);

      if (blips.size() > maxTrail) {
        blips.remove(0);
        alphas.remove(0);
      }
    }
  }

  // --- Draw blips with fading ---
  for (int i = 0; i < blips.size(); i++) {
    fill(255, 0, 0, alphas.get(i));
    noStroke();
    ellipse(blips.get(i).x, blips.get(i).y, 10, 10);
    alphas.set(i, max(0, alphas.get(i) - 3));
  }
}

void drawRadarGrid() {
  stroke(0, 255, 0, 100);
  noFill();
  for (int r = 50; r <= 250; r += 50) {
    ellipse(0, 0, r*2, r*2);
  }

  stroke(0, 255, 0, 50);
  line(-width/2, 0, width/2, 0);
  line(0, -height/2, 0, height/2);
}

void fetchData() {
  try {
    JSONObject json = loadJSONObject(url);
    if (json == null) return;

    JSONArray feeds = json.getJSONArray("feeds");
    if (feeds == null || feeds.size() == 0) return;

    JSONObject lastFeed = feeds.getJSONObject(feeds.size() - 1);

    if (!lastFeed.isNull("field1"))
      distance = float(lastFeed.getString("field1"));

    if (!lastFeed.isNull("field2"))
      angle = float(lastFeed.getString("field2"));

    println("Distance: " + distance + " cm, Angle: " + angle + "°");

  } catch (Exception e) {
    println("Error fetching data: " + e);
  }
}
