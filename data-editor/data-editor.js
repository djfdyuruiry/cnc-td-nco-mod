const { readFileSync, writeFileSync } = require("fs")
const { resolve } = require("path")

const SRC_PATH = resolve(`${__dirname}/../TIBERIANDAWN/`)

function rewriteRule(
  sourceFileName,
  typeName,
  ruleFunction,
  iniNameTag,
  ruleVaueCapture,
  ruleNameTag
) {
  const sourceFile = `${SRC_PATH}/${sourceFileName}.CPP`
  const sourceCode = readFileSync(sourceFile, "utf-8")

  const ruleRegex = new RegExp(
      `static ${typeName} const ([\\S\\s\\r]+?^\\s+)(".+?")(.+\\/\\/ ${iniNameTag}[\\S\\s\\r]+?^\\s+)${ruleVaueCapture}(,.+\\/\\/ ${ruleNameTag}[\\S\\s\\r]+?\\);[\\n\\r])`,
      "mg"
  )

  const rewrittenCode = sourceCode.replace(ruleRegex, `static ${typeName} const $1$2$3${ruleFunction}($2, $4)$5`)

  writeFileSync(sourceFile, rewrittenCode)
}

const BOOLEAN_RULE_CAPTURE = "(true|false)"
const INT_RULE_CAPTURE = "(\\d+)"
const RULES = [
  ["Read_Risk_Reward", "(\\d+,\\d+)",  "Risk/Reward of this infantry unit."]
]

function main() {
  RULES.forEach(i =>
    rewriteRule(
      "IDATA",
      "InfantryTypeClass",
      i[0],
      "INI name for infantry",
      i[1],
      i[2]
    )
  )
}

main()
