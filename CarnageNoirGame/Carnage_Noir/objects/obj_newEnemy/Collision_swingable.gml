if !instance_exists(player) exit
if other.swing_speed>0 or other.swing_speed<0 
{
	if intersection(x,y,player.x,player.y,4)<0 
	{
		other.image_index=1
		if other.object_index=sword or other.object_index=axe or other.object_index=machete
		{
			splitt=1
			splitt_dir=other.swing_speed/10
		}
		else
		{
			sound_play(crush)
			clubbed=1
			splitt_dir=other.swing_speed/10
		}
		instance_destroy()
	}
}


if other.thrown {
	if other.speed>2 sound_play(throw_hit)
	if other.speed>2
	{
	door_hit=1
	index=0
	sprite_index=get_up
	direction=other.direction
	my_speed=2+random(2)
	if armed=1
	{
	armed=0
	my_i=instance_create(x,y,gun_to_drop)
	my_i.ammo_type = ammo_type;
	my_i.image_angle=direction
	my_i.direction=direction-180-20+random(40)
	my_i.speed=1+random(0.5)
	}
	}
	other.speed=other.speed/2
	other.direction-=180-20+random(40)
}