-- 讀入設定文件（索引）

local cf = assert(io.open("jstml.conf", "r"))
local cfcntn = cf:read("a")
cf:close()
cfntn = string.gsub(cfcntn, "\n", ",")
cfntn = "{" .. cfntn .. "}"

-- 開啟輸出目錄流
local wobj = assert(io.open("jsindex.ind", "w"))

-- 循環處理、實際調用jstml
for _, fnc in ipairs(cfntn) do
  local ofn = string.gsub(fnc, "%.%a+", ".tex")
  os.execute("./jstml.wasm -o " .. ofn .. " " .. fnc)
  wobj:write("\\input{" .. ofn .. "}\n")
  print(string.format("[%-20s]->[%20s]<>[jsindex.ind]", fnc, ofn))
end

-- 養成好習慣、關閉輸出流
wobj:close()

-- 調用LuaTeX讀入dirver style file排版最終結果
os.execute("lualatex jsdvr.tex")

-- FAREWELL
print("Run finished, terminating...")
