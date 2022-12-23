make all

TARGET='./build/image-transformer'
TESTS='./tests'

for test_dir in ${TESTS}/*; do
    echo "-- TEST '${test_dir}' --"
    $TARGET "${test_dir}/input.bmp" "${test_dir}/output_c.bmp" "${test_dir}/output_asm.bmp" 
done