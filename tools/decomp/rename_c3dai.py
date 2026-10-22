#!/usr/bin/env python3
# rename_c3dai.py [--arrow] files...: Negatron C3DAI member / slot names -> the Neutron.exe C3DAI's (commit d66f01a).
# Plain: bare tokens (in files of C3DAI subclasses); --arrow: only "->name" (files that use C3DAI pointers; check the diff).
import re,sys
M={'f_960':'m_time','f_964':'m_target','f_968':'m_speed','f_96c':'m_state','f_970':'m_prev_state','f_974':'m_target_id',
'f_978':'m_offset','f_988':'m_dest_vec','f_998':'m_arrived','f_99c':'m_arrived_time','f_9a0':'m_dest_dist','f_9a4':'m_stuck_dist',
'f_9a8':'m_visible_range','f_9ac':'m_patrol_point','f_a10':'m_target_name','f_a74':'m_anim_none','f_a9c':'m_anim_wag',
'f_ac4':'m_anim_run','f_aec':'m_anim_fly','f_b14':'m_anim_sit','f_b3c':'m_anim_walk','f_b64':'m_anim_walk','f_b8c':'m_dest',
'f_b94':'m_prev_point','f_b98':'m_fov','f_b9c':'m_turn_rates','f_bac':'m_time2','f_bb0':'m_keep_point','f_bb4':'m_dest_point',
'f_bc4':'m_8f8','f_bd4':'m_hover','f_bd5':'m_909','f_bd8':'m_wait_time','f_bdc':'m_no_turn','f_be0':'m_saved_state',
'f_be4':'m_saved_offset','f_bf4':'m_saved_target','f_bf8':'m_attack_time','f_bfc':'m_stuck_timer','f_c00':'m_seen_time',
'f_c04':'m_938','f_c08':'m_ai_state','f_c10':'m_flee_dist','f_c14':'m_arrive_dist','f_c18':'m_move_forward',
'f_c1c':'m_keep_fleeing','f_c20':'m_wander_started','f_c24':'m_wander_time','f_c28':'m_958','f_c2c':'m_wander_range',
'f_c30':'m_last_yaw_step','f_c34':'m_see_through_walls','f_c38':'m_anims_disabled','f_c3c':'m_flee_time',
'f_c40':'m_state_locked','f_c44':'m_974','f_c48':'m_check_stuck','f_c4c':'m_hold_pos','f_c5c':'m_held',
'f_c5d':'m_hold_when_touched','f_c60':'m_990'}
E={106:72,107:73,108:74,109:75,110:76,111:77,112:78,113:79,114:80,115:81,116:82,123:87,124:88,125:89,126:90,127:91,128:92,129:93,130:94}
arrow='--arrow' in sys.argv
for f in [a for a in sys.argv[1:] if a!='--arrow']:
    s=open(f).read(); o=s
    pre=r'(?<=->)' if arrow else r'(?<![\w.>])'
    for k,v in M.items(): s=re.sub(pre+r'\b'+k+r'\b',v,s)
    def ev(m):
        n=int(m.group(1))
        if n in E: return 'ev%d'%E[n]
        if 106<=n<=143: return 'n2_ev%d'%n
        return m.group(0)
    s=re.sub(pre+r'\bev(1[0-4]\d)\b',ev,s)
    s=re.sub(pre+r'\bFindBestPath\b','n2_FindBestPath',s)
    if s!=o: open(f,'w').write(s); print('renamed',f)
