import os

# 设置你要扫描的文件扩展名
EXTENSIONS = {'.ino', '.c', '.h', '.cpp', '.hpp'}

def create_ai_context(output_file="project_context.txt"):
    with open(output_file, 'w', encoding='utf-8') as outfile:
        # 1. 先写入文件目录树
        outfile.write("[Project File Structure]\n")
        for root, dirs, files in os.walk("."):
            for file in files:
                if any(file.endswith(ext) for ext in EXTENSIONS):
                    path = os.path.join(root, file)
                    outfile.write(f"{path}\n")
        outfile.write("\n" + "="*40 + "\n\n")

        # 2. 写入文件内容
        for root, dirs, files in os.walk("."):
            for file in files:
                if any(file.endswith(ext) for ext in EXTENSIONS):
                    file_path = os.path.join(root, file)
                    outfile.write(f"--- BEGIN FILE: {file_path} ---\n")
                    try:
                        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                            outfile.write(f.read())
                    except Exception as e:
                        outfile.write(f"Error reading file: {e}")
                    outfile.write(f"\n--- END FILE: {file_path} ---\n\n")
    
    print(f"完成！请将 {output_file} 的内容发送给 AI。")

if __name__ == "__main__":
    create_ai_context()
