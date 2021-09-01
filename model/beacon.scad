//-----------------------------
// beacon
//-----------------------------
DRAW_BEACONS = true;
LARGE_BEACON_ONLY = false;
SMALL_BEACON_ONLY = false;
DRAW_BASE = true;
DRAW_BOARD_SUPPORT = false;

SHAFT_RADIUS = 5;
SHAFT_TUNNEL_RADIUS = 2.7;
BALL_RADIUS = 20;
CONE_HEIGHT = 15;
CONE_MAJOR_RADIUS = 15;
OFFSET_ANGLE = 13;
WALL_THICKNESS = 6;
BASE_HEIGHT = 20;

module beacon(x, y, z, h, draw_sphere = true)
{
   BALL_TO_CONE_DEPTH = 4.5;
   
   difference()
   {
      color([.1, 0, 1])
      {
         translate([x, y, z + h / 2])
         cylinder(h, SHAFT_RADIUS, SHAFT_RADIUS, true, $fn=100);

         translate([x, y, z + h + (CONE_HEIGHT / 2)])
         cylinder(CONE_HEIGHT, SHAFT_RADIUS, CONE_MAJOR_RADIUS, true, $fn=100);
      }
   
      color([0, 1, 0])
      {
         translate([0, 0, z + (h + 12) / 2])
         cylinder(h + 24, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true, $fn=100);
         
         translate([x, y, h + z + (CONE_HEIGHT / 2) + BALL_TO_CONE_DEPTH])
         cylinder(CONE_HEIGHT, SHAFT_RADIUS, CONE_MAJOR_RADIUS, true, $fn=100);
         
         translate([0, 0, z + h + SHAFT_RADIUS + BALL_RADIUS + BALL_TO_CONE_DEPTH], $fn=100)
         sphere(BALL_RADIUS);
      }
   }
   
   if (draw_sphere == true)
   {
      color([1, 1, 1])
      translate([0, 0, z + h + SHAFT_RADIUS + BALL_RADIUS + BALL_TO_CONE_DEPTH])
      sphere(BALL_RADIUS, $fn=360);
   }
}

module base(x, y, z, r)
{
   difference()
   {
      color([0, 0, 1])
      translate([x, y, z])
      sphere(r, $fn=100);
      
      sphere(r - WALL_THICKNESS);
      
      translate([x, y, -r / 2])
      cube([r * 2 + 1, r * 2 + 1, r], true);
      
      translate([0, 0, r - SHAFT_RADIUS / 2])
      {
         cylinder(WALL_THICKNESS * 2, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true, $fn=100);
         cylinder(WALL_THICKNESS * .8, SHAFT_RADIUS, SHAFT_RADIUS, true, $fn=100);
      }
      
      for (rx = [OFFSET_ANGLE, -OFFSET_ANGLE])
      {
         for (ry = [OFFSET_ANGLE, -OFFSET_ANGLE])
         {
            rotate([rx, ry, 0])
            translate([0, 0, r - SHAFT_RADIUS / 2])
            {
               cylinder(WALL_THICKNESS * 2, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true, $fn=100);
               cylinder(WALL_THICKNESS * .8, SHAFT_RADIUS, SHAFT_RADIUS, true, $fn=100);
            }
         }
      }      
     
   }
   
   difference()
   {
      color([0, 0, 1])
      translate([x, y, z - BASE_HEIGHT / 2])
      cube([r * 2, r * 2, BASE_HEIGHT], true);
      
      translate([0, 0, z - BASE_HEIGHT / 2 - 1])
      cylinder(BASE_HEIGHT + 4, r - WALL_THICKNESS, r - WALL_THICKNESS, true);

      foot_offset = r - WALL_THICKNESS - 6;
      foot_radius = 6;
      
      for (x1 = [foot_offset, -foot_offset])
      {
         translate([x1, 0, z - BASE_HEIGHT - foot_radius / 4])
         rotate([90, 0, 0])
         cylinder(r * 2 + 2, foot_radius, foot_radius, true);
      }
      
      for (y1 = [foot_offset, -foot_offset])
      {
         translate([0, y1, z - BASE_HEIGHT - foot_radius / 4])
         rotate([0, 90, 0])
         cylinder(r * 2 + 2, foot_radius, foot_radius, true);
      
      }

      translate([0, 0, z - BASE_HEIGHT + foot_radius / 4])
      {
         cube ([foot_offset * 2, r * 2 + 4, foot_radius], true);
         cube ([r * 2 + 4, foot_offset * 2, foot_radius], true);
      }
      
      color([1, 0, 0])
      translate([0, 23, -BASE_HEIGHT + 4 + 1.5])
      cube([17.5, 32, 3], true);
      

      color([0, 0, 1])
      translate([0, -33, -BASE_HEIGHT + 5])
      rotate([90, 90, 0])
      cylinder(20, 5, 5, true);
   }
}

if (DRAW_BASE == true)
{
   base(0, 0, 0, 40);
}

if (DRAW_BEACONS == true)
{
   if (LARGE_BEACON_ONLY == true)
   {
      rotate([0, 0, 0])
      beacon(0, 0, 35, 100, false);
   }
   else if (SMALL_BEACON_ONLY == true)
   {
      rotate([0, 0, 0])
      beacon(0, 0, 35, 80, false);
   }
   else if (DRAW_BOARD_SUPPORT == false)
   {
      rotate([0, 0, 0])
      beacon(0, 0, 35, 100, true);

      for (rx = [OFFSET_ANGLE, -OFFSET_ANGLE])
      {
         for (ry = [OFFSET_ANGLE, -OFFSET_ANGLE])
         {
            rotate([rx, ry, 0])
            beacon(0, 0, 35, 80, true);
         }
      }
   }

   if (DRAW_BOARD_SUPPORT == true)
   {
      // board support
      color([1, 0, 0])
      translate([0, 18, -BASE_HEIGHT + 4 + 1.5])
      cube([17.5, 41, 3], true);
   }
}
