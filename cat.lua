--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

function limb (x, y, name)
  nlimb = gr.node('nlimb')
  upperlimb = gr.sphere('upperlimb')
  nlimb:add_child(upperlimb)

  jmid = gr.joint(name..'jmid', {0, 0, 10}, {0, 0, 10})
  nlimb:add_child(jmid)
  nmid = gr.node('nmid')
  jmid:add_child(nmid)

  midlimb = gr.sphere('mid')
  nmid:add_child(midlimb)

  jlow = gr.joint(name..'jlow', {0, 0, 10}, {0, 0, 10})
  nmid:add_child(jlow)
  nlow = gr.node('nlow')

  jlow:add_child(nlow)
  lowlimb = gr.sphere('low')
  nlow:add_child(lowlimb)

  upperlimb:scale(0.1, 0.8, 0.1)
  midlimb:scale(0.1, 0.5, 0.1)
  lowlimb:scale(0.2, 0.2, 0.2)

  upperlimb:set_material(green)
  midlimb:set_material(blue)
  lowlimb:set_material(red)

  upperlimb:translate(x*10, y+3, 0)
  midlimb:translate(x*10, y+1.3, 0)
  lowlimb:translate(x*5, y-3, 0)

  return nlimb
end

function leg (x, y, name)
  nlimb = gr.node('nlimb')
  upperlimb = gr.sphere('upperlimb')
  nlimb:add_child(upperlimb)

  jmid = gr.joint(name..'jmid', {0, 0, 10}, {0, 0, 10})
  nlimb:add_child(jmid)
  nmid = gr.node('nmid')
  jmid:add_child(nmid)

  midlimb = gr.sphere('mid')
  nmid:add_child(midlimb)

  jlow = gr.joint(name..'jlow', {0, 0, 10}, {0, 0, 10})
  nmid:add_child(jlow)
  nlow = gr.node('nlow')

  jlow:add_child(nlow)
  lowlimb = gr.sphere('low')
  nlow:add_child(lowlimb)

  upperlimb:scale(0.1, 0.8, 0.1)
  midlimb:scale(0.1, 0.5, 0.1)
  lowlimb:scale(0.5, 0.2, 0.2)

  upperlimb:set_material(green)
  midlimb:set_material(blue)
  lowlimb:set_material(red)

  upperlimb:translate(x*10, y+3.8, 0)
  midlimb:translate(x*10, y+0.5, 0)
  lowlimb:translate(x*3, y-10, 0)

  return nlimb
end

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

--torso
ntorso = gr.node('ntorso')
rootnode:add_child(ntorso)

torso = gr.sphere('torso')
ntorso:add_child(torso)
torso:scale(1.0, 1.5, 1.0)
torso:translate(0, 0.3, 0)
torso:set_material(blue)

nshoulder = gr.node('nshoulder')
ntorso:add_child(nshoulder)
shoulder = gr.sphere('shoulder')
nshoulder:add_child(shoulder)
shoulder:set_material(red)
shoulder:scale(2, 0.5, 0.5)
shoulder:translate(0, 3, 0)

nhips = gr.node('nhips')
ntorso:add_child(nhips)

hips = gr.sphere('hips')
nhips:add_child(hips)
hips:set_material(red)
hips:scale(1.2, 0.5, 0.5)
hips:translate(0, -2, 0)

nneck = gr.node('nneck')
nshoulder:add_child(nneck)

jneck = gr.joint('jneck', {0, 0, 10}, {0, 0, 10})
nneck:add_child(jneck)

--neck
neck = gr.sphere('neck')
nneck:add_child(neck)
neck:scale(0.2, 0.2, 0.2)
neck:translate(0, 10, 0.0)
neck:set_material(green)

--head
nhead = gr.node('nhead')
jneck:add_child(nhead)

head = gr.sphere('head')
nhead:add_child(head)
head:set_material(white)
head:scale(0.5, 0.5, 0.5)
head:translate(0, 5.0, 0.0)

nose = gr.sphere('nose')
nhead:add_child(nose)
nose:set_material(red)
nose:scale(0.1, 0.1, 1)
nose:translate(0.0, 25, 0.0)
  
larm = gr.joint('larm', {0, 0, 10}, {0, 0, 10})
nshoulder:add_child(larm)
larm:add_child(limb(-2, -2, 'larm'));

rarm = gr.joint('rarm', {0, 0, 10}, {0, 0, 10})
nshoulder:add_child(rarm)
rarm:add_child(limb(2, -2, 'rarm'));

lleg = gr.joint('lleg', {0, 0, 10}, {0, 0, 10})
nhips:add_child(lleg)
lleg:add_child(leg(-1, -6, 'lleg'));

rleg = gr.joint('rleg', {0, 0, 10}, {0, 0, 10})
nhips:add_child(rleg)
rleg:add_child(leg(1, -6, 'rleg'));

rootnode:translate(0.0, 0.0, -12.0)
rootnode:rotate('y', -20.0)

return rootnode
