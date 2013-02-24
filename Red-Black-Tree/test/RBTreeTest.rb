a = (0...ARGV[0].to_i).to_a
f = File.new "rbt_test.c", "w"
a.shuffle!

f.puts "
#define VERBOSE 1
#include \"RedBlackTree.h\"

int main() {
  RBTree tree;
  init_rbtree(&tree, #{ ARGV[0].to_i });
"

a.each { |e| f.puts "  rbtree_add(&tree, #{ e });" }
f.puts "\n"
a.shuffle!
a.each { |e| f.puts "  printf(\"%10d => %d \\n\", #{ e }, rbtree_remove(&tree, #{ e }));" }

f.puts "

  return 0;
}"

f.close
system "gcc rbt_test.c"
system "./a.out"
