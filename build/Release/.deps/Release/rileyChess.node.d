cmd_Release/rileyChess.node := ln -f "Release/obj.target/rileyChess.node" "Release/rileyChess.node" 2>/dev/null || (rm -rf "Release/rileyChess.node" && cp -af "Release/obj.target/rileyChess.node" "Release/rileyChess.node")
