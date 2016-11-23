
#include <cstdio>
#include <cstdlib>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_PREFIX_TRIE
#include <cedarpp.h>
#else
#include <cedar.h>
#endif
 
//typedef cedar::da<int> cedar_t;

template<typename T>
inline void insert_key(cedar::da<T> &t, const char* key, T new_value)
{
  t.update(key) = new_value;
}
template<typename T>
inline bool lookup_key(cedar::da<T> &t, const char* key)
{
  return t.exactMatchSearch<int>(key) >= 0;
}

static const size_t NUM_RESULT = 1024;
// --------------------------------------------------------------------------------------------------------------------
int main (int argc, char **argv)
{
  cedar::da<int> trie;

  insert_key(trie, "我爱你", 0);
  insert_key(trie, "我爱他", 0);
  insert_key(trie, "我爱她", 0);
  insert_key(trie, "爱你", 0);
//   insert_key(trie, "ab", 0);
//   insert_key(trie, "abcd", 1);
//   insert_key(trie, "abc", 2);
//   insert_key(trie, "dc", 3);

  cedar::da<int>::result_pair_type   result_pair[NUM_RESULT];
  cedar::da<int>::result_triple_type result_triple[NUM_RESULT];
  std::fprintf(stdout, "commonPrefixSearch ():\n");
//  const char * line = "ab";
  const char * line = "我爱";
  if (const size_t n = trie.commonPrefixSearch(line, result_pair, NUM_RESULT)) {
	  std::fprintf(stdout, "%s: found, num=%ld ", line, n);
	  for (size_t i = 0; i < n; ++i) {
		  if (i >= NUM_RESULT) { std::fprintf(stdout, " ..truncated"); break; }
		  std::fprintf(stdout, " %d:%ld", result_pair[i].value, result_pair[i].length);
	  }
	  std::fprintf(stdout, "\n");
  }
  else {
	  std::fprintf(stdout, "%s: not found\n", line);
  }
  char suffix[1024];
  std::fprintf(stdout, "commonPrefixPredict ():\n");
  if (const size_t n = trie.commonPrefixPredict(line, result_triple, NUM_RESULT)) {
	  std::fprintf(stdout, "%s: found, num=%ld ", line, n);
	  for (size_t i = 0; i < n; ++i) {
		  if (i >= NUM_RESULT) { std::fprintf(stdout, " ..truncated"); break; }
		  trie.suffix(suffix, result_triple[i].length, result_triple[i].id);
		  std::fprintf(stdout, " %d:%ld:%ld:%s", result_triple[i].value, result_triple[i].length, result_triple[i].id, suffix);
	  }
	  std::fprintf(stdout, "\n");
  }
  else {
	  std::fprintf(stdout, "%s: not found\n", line);
  }
  if(lookup_key(trie, line)) {
    printf("lookup_key1 %d \n",trie.exactMatchSearch<int>("test"));
  }
  if (lookup_key(trie, "爱他")) {
    printf("lookup_key2 none %d \n", trie.exactMatchSearch<int>("none"));
  } else {
    printf("lookup_key none %d \n", trie.exactMatchSearch<int>("none"));
  }
  return 0;
}
