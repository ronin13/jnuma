/*
 * Copyright 2012 Taro L. Saito
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE

#include "NumaNative.h"

#include <numa.h>

#include <sched.h>
#include <stdio.h>
#include <stdint.h>
#include <sched.h>
#include <errno.h>

inline void throwException(JNIEnv *env, jobject self, int errorCode) {
  do {
    jclass c = (*env)->FindClass(env, "xerial/jnuma/NumaNative");
    if(c == 0) break;
    jmethodID m = (*env)->GetMethodID(env, c, "throwError", "(I)V");
    if(m == 0) break;
    (*env)->CallVoidMethod(env, self, m, (jint) errorCode);
    return;
  } while (0);

  fprintf(stderr, "Error happens though, cannot throw an exception");
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    numaAvailable
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_xerial_jnuma_NumaNative_isAvailable
    (JNIEnv *env, jobject obj) {
  return numa_available() != -1;
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    maxNode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xerial_jnuma_NumaNative_maxNode
    (JNIEnv *env, jobject obj) {
  return numa_max_node();
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    Node
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xerial_jnuma_NumaNative_currentNode
    (JNIEnv *env, jobject obj) {
  return numa_node_of_cpu(sched_getcpu());
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    currentCpu
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xerial_jnuma_NumaNative_currentCpu
    (JNIEnv *env, jobject obj) {
  return sched_getcpu();
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    nodeSize
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_nodeSize
    (JNIEnv *env, jobject obj, jint node) {
  return (jlong) numa_node_size((int) node, NULL);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    freeSize
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_freeSize
    (JNIEnv *env, jobject obj, jint node) {
  long free = 0;
  numa_node_size((int) node, &free);
  return free;
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    distance
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_xerial_jnuma_NumaNative_distance
    (JNIEnv *env, jobject obj, jint node1, jint node2) {
  return numa_distance(node1, node2);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    allocate
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_allocate
    (JNIEnv *env, jobject obj, jlong capacity) {
  void* mem = numa_alloc((size_t) capacity);
  if(mem != NULL) {
    return (jlong) mem;
  }
  throwException(env, obj, 11);
  return 0L;
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    allocateLocal
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_allocateLocal
    (JNIEnv *env, jobject obj, jlong capacity) {
  void* mem = numa_alloc_local((size_t) capacity);
  if(mem != NULL) {
    return (jlong) mem;
  }
  throwException(env, obj, 11);
  return 0L;
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    allocateOnNode
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_allocateOnNode
    (JNIEnv *env, jobject obj, jlong capacity, jint node) {
  void* mem = numa_alloc_onnode((size_t) capacity, node);
  if(mem != NULL) {
    return (jlong) mem;
  }
  throwException(env, obj, 11);
  return 0L;
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    allocateInterleaved
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_xerial_jnuma_NumaNative_allocateInterleaved
    (JNIEnv *env, jobject obj, jlong capacity) {
  void* mem = numa_alloc_interleaved((size_t) capacity);
  if(mem != NULL) {
    return (jlong) mem;
  }
  throwException(env, obj, 11);
  return 0L;
}

JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_free
    (JNIEnv *env, jobject jobj, jlong address, jlong capacity) {
  if(address != 0) {
    numa_free((void*) address, (size_t) capacity);
  }
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    preferredNode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xerial_jnuma_NumaNative_preferredNode
    (JNIEnv *env, jobject obj) {
  return (jint) numa_preferred();
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    setLocalAlloc
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_setLocalAlloc
    (JNIEnv *env, jobject obj) {
  numa_set_localalloc();
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    getAffinity
 * Signature: (I[JI)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_getAffinity
    (JNIEnv *env, jobject obj, jint pid, jlongArray maskBuf, jint numCPUs) {
  uint64_t* in = (uint64_t*) (*env)->GetPrimitiveArrayCritical(env, (jarray) maskBuf, 0);
  cpu_set_t mask;
  if (in == 0) {
    throwException(env, obj, 10);
  }
  CPU_ZERO(&mask);
  const int ret = sched_getaffinity(0, sizeof(mask), &mask);
  if (ret < 0) {
    throwException(env, obj, ret);
  }
  for (int i = 0; i < numCPUs; ++i) {
    if (CPU_ISSET(i, &mask))
      in[i / 64] |= (uint64_t) (((uint64_t) 1) << (i % 64));
  }
  (*env)->ReleasePrimitiveArrayCritical(env, (jarray) maskBuf, (void*) in, (jint) 0);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    setAffinity
 * Signature: (I[JI)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_setAffinity
    (JNIEnv *env, jobject obj, jint pid, jlongArray maskBuf, jint numCPUs) {
  uint64_t* in = (uint64_t*) (*env)->GetPrimitiveArrayCritical(env, (jarray) maskBuf, 0);
  cpu_set_t mask;
  CPU_ZERO(&mask);
  for (int i = 0; i < numCPUs; ++i) {
    if (in[i / 64] & ((uint64_t) 1 << (i % 64)))
      CPU_SET((int) i, &mask);
  }
  (*env)->ReleasePrimitiveArrayCritical(env, (jarray) maskBuf, (void*) in, (jint) 0);
  const int ret = sched_setaffinity(0, sizeof(cpu_set_t), &mask);
  if (ret < 0) {
    throwException(env, obj, errno);
  }
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    setPreferred
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_setPreferred
    (JNIEnv *env, jobject obj, jint node) {
  numa_set_preferred((int) node);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    runOnNode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_runOnNode
    (JNIEnv *env, jobject obj, jint node) {
  numa_run_on_node((int) node);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    toNode
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_toNode__JII
    (JNIEnv *env, jobject obj, jlong address, jint length, jint node) {
  numa_tonode_memory((void*) address, (size_t) length, (int) node);
}

/*
 * Class:     xerial_jnuma_NumaNative
 * Method:    toNodeMemory
 * Signature: (Ljava/lang/Object;II)V
 */
JNIEXPORT void JNICALL Java_xerial_jnuma_NumaNative_toNode__Ljava_lang_Object_2II
    (JNIEnv *env, jobject obj, jobject array, jint length, jint node) {
  void* buf = (void*) (*env)->GetPrimitiveArrayCritical(env, (jarray) array, 0);
  numa_tonode_memory(buf, (size_t) length, (int) node);
  (*env)->ReleasePrimitiveArrayCritical(env, (jarray) array, buf, (jint) 0);
}

