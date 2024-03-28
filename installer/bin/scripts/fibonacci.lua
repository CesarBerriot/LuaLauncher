function fibonacci(n)
  local function inner(m)
    if m < 2 then
      return m
    end
    return inner(m-1) + inner(m-2)
  end
  return inner(n)
end

for i=0,24 do
  print("fibonnaci value of "..tostring(i).." : "..tostring(fibonacci(i)))
end