for item in $(du -shc ./release_BUILD/llvm/lib/* | grep "libLLVM.*\.a" | sed -e "s/.*libLLVM/LLVM/g" -e "s/\.a//g")
    do
        echo "list(APPEND LLVM_MC_JIT_LINK_LIBS $item)"
done
for item in $(du -shc ./release_BUILD/llvm/lib/* | grep "libclang.*\.a" | sed -e "s/.*libclang/clang/g" -e "s/\.a//g")
    do
        echo "list(APPEND LLVM_MC_JIT_LINK_LIBS $item)"
done

for item in $(du -shc ./release_BUILD/llvm/lib/* | grep "liblldb.*\.a" | sed -e "s/.*liblldb/lldb/g" -e "s/\.a//g")
    do
        echo "list(APPEND LLVM_MC_JIT_LINK_LIBS $item)"
done
