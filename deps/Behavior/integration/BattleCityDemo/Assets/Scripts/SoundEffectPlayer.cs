/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class TrackPlayer
{
	public TrackPlayer()
	{
		track = -1; 
		start = 0.0f; 
		end = 1.0f; 
		loop = false; 
		clip = null; 
		p = start; 
		playing = false;
		loopCount = 0;
	}
	public int track;
	public float start;
	public float end;
	public bool loop;
	public AudioClip clip;

	public float p;
	public bool playing;
	public int loopCount;
}

public class SoundEffectPlayer : Singleton<SoundEffectPlayer>
{
	private List<AudioSource> audioSources = new List<AudioSource>();
	const int priorityLevel = 6;

	private List<TrackPlayer> tracks = new List<TrackPlayer>();
	void Awake()
	{
		for(int i = 0; i < priorityLevel; ++i)
		{
			AudioSource audioPlayer = gameObject.AddComponent<AudioSource>();
			audioPlayer.playOnAwake = false;
			audioSources.Add(audioPlayer);

			TrackPlayer tp = new TrackPlayer();
			tracks.Add(tp);
		}
	}
	// Use this for initialization
	void Start()
	{
	
	}
	
	// Update is called once per frame
	void Update()
	{
//		float deltaTime = Time.deltaTime;
//		for(int i = 0; i < tracks.Count; ++i)
//		{
//			TrackPlayer tp = tracks[i];
//			if(tp.clip == null || tp.track < 0)
//				continue;
//
//			float clipLength = tp.clip.length;
//			float s = tp.start*clipLength;
//			float e = tp.end*clipLength;
//
//			if(tp.playing)
//			{
//				bool looped = false;
//				float newP = tp.p + deltaTime;
//				if(newP > e)
//				{
//					tp.loopCount++;
//					tp.p = s;
//					looped = true;
//				}
//				else
//				{
//					tp.p = newP;
//				}
//
//				if(tp.loop)
//				{
//					if(looped)
//					{
//						if(!audioSources[tp.track].isPlaying)
//							audioSources[tp.track].Play();
//						audioSources[tp.track].time = tp.p;
//					}
//				}
//				else
//				{
//					if(tp.loopCount > 0)
//					{
//						audioSources[tp.track].Stop();
//						tp.playing = false;
//						tp.p = s;
//					}
//				}
//			}
//			else
//			{
//				if(!tp.playing)
//				{
//					tp.p = s;
//					tp.playing = true;
//
//					audioSources[tp.track].clip = tp.clip;
//					audioSources[tp.track].Play();
//					audioSources[tp.track].time = s;
//				}
//			}
//		}
	}

	public void playSound(string soundName, int priority, float start_, float end, bool loop)
	{
		AudioClip clip = ResourceManager.Instance.getAudioClip(soundName);
		audioSources[priority].clip = clip;
		audioSources[priority].Play();
//		if(!audioSources[priority].isPlaying)
//		{
//			TrackPlayer tp = tracks[priority];
//
//			tp.track = priority;
//			tp.start = start_;
//			tp.end = end;
//			tp.loop = loop;
//			tp.clip = clip;
//
//			tp.p = 0.0f;
//			tp.playing = false;
//			tp.loopCount = 0;
//		}
	}
}
