//-----------------------------
// beacon
//-----------------------------
DRAW_BEACONS = true;


SHAFT_RADIUS = 4;
SHAFT_TUNNEL_RADIUS = 2;
BALL_RADIUS = 20;
CONE_HEIGHT = 15;
CONE_MAJOR_RADIUS = 15;
OFFSET_ANGLE = 15;
WALL_THICKNESS = 6;

module beacon(x, y, z, h, draw_sphere = true)
{
   BALL_TO_CONE_DEPTH = 4.5;
   
   difference()
   {
      color([.1, 0, 1])
      {
         translate([x, y, z + h / 2])
         cylinder(h, SHAFT_RADIUS, SHAFT_RADIUS, true);

         translate([x, y, z + h + (CONE_HEIGHT / 2)])
         cylinder(CONE_HEIGHT, SHAFT_RADIUS, CONE_MAJOR_RADIUS, true);
      }
   
      color([0, 1, 0])
      {
         translate([0, 0, z + (h + 12) / 2])
         cylinder(h + 24, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true);
         
         translate([x, y, h + z + (CONE_HEIGHT / 2) + BALL_TO_CONE_DEPTH])
         cylinder(CONE_HEIGHT, SHAFT_RADIUS, CONE_MAJOR_RADIUS, true);
         
         translate([0, 0, z + h + SHAFT_RADIUS + BALL_RADIUS + BALL_TO_CONE_DEPTH])
         sphere(BALL_RADIUS);
      }
   }
   
   if (draw_sphere == true)
   {
      color([1, 1, 1])
      translate([0, 0, z + h + SHAFT_RADIUS + BALL_RADIUS + BALL_TO_CONE_DEPTH])
      sphere(BALL_RADIUS);
   }
}

module base(x, y, z, r)
{
   
   difference()
   {
      color([0, 0, 1])
      translate([x, y, z])
      sphere(r);
      
      sphere(r - WALL_THICKNESS);
      
      translate([x, y, -r / 2])
      cube([r * 2 + 1, r * 2 + 1, r], true);
      
      translate([0, 0, r - SHAFT_RADIUS / 2])
      {
         cylinder(WALL_THICKNESS * 2, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true);
         cylinder(WALL_THICKNESS * .7, SHAFT_RADIUS, SHAFT_RADIUS, true);
      }
      
      for (rx = [OFFSET_ANGLE, -OFFSET_ANGLE])
      {
         for (ry = [OFFSET_ANGLE, -OFFSET_ANGLE])
         {
            rotate([rx, ry, 0])
            translate([0, 0, r - SHAFT_RADIUS / 2])
            {
               cylinder(WALL_THICKNESS * 2, SHAFT_TUNNEL_RADIUS, SHAFT_TUNNEL_RADIUS, true);
               cylinder(WALL_THICKNESS * .7, SHAFT_RADIUS, SHAFT_RADIUS, true);
            }
         }
      }      
     
   }
}

base(0, 0, 0, 25);

if (DRAW_BEACONS == true)
{
   rotate([0, 0, 0])
   beacon(0, 0, 23, 100, true);

   for (rx = [OFFSET_ANGLE, -OFFSET_ANGLE])
   {
      for (ry = [OFFSET_ANGLE, -OFFSET_ANGLE])
      {
         rotate([rx, ry, 0])
         beacon(0, 0, 23, 80, true);
      }
   }
}
