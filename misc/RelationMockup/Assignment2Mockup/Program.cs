using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment2Mockup
{
    public class _Relation
    {
        public string Name;
        public _Types._Int AutoKey;
        public _Column<_Types._Int> IDColumn;
        public LinkedList<_Column> Columns;

        public _Relation(string Name)
        {
            this.Name = Name;
            AutoKey = new _Types._Int(0);
            IDColumn = new _Column<_Types._Int>("ID");
            Columns = new LinkedList<_Column>();
        }

        public void AddColumn(_Column C)
        {
            Columns.AddLast(C);
        }
        public void AddRow(_Types[] args)
        {
            if (args.Count() != Columns.Count())
                return;

            AutoKey.SetData((int)AutoKey.GetData()+1);
            IDColumn.AddRow(AutoKey);

            for(int i =0; i < Columns.Count();i++)
                Columns.ElementAt(i).AddRow(args[i]);
        }

        public void Print()
        {
            Console.Out.WriteLine(" ========= " +Name + " ========= ");
            for (int i = 0; i < IDColumn.GetSize(); i++)
            {
                for (int k = 0; k < Columns.Count(); k++)
                {
                    Columns.ElementAt(k).Print(i);
                    Console.Out.Write(" - ");
                }
                Console.Out.WriteLine("\n-------------------\n");
            }
        }
    }

    public class _Types
    {
        object Data;

        public _Types()
        {
            Data = null;
        }
        public _Types(object Data)
        {
            this.Data = Data;
        }
        public object GetData()
        {
            return Data;
        }
        public void SetData(object Data)
        {
            this.Data = Data;
        }

        public class _VarChar : _Types
        {
            public _VarChar(string Data)
                : base(Data)
            {

            }
        }

        public class _Int : _Types
        {
            public _Int(int Data)
                : base(Data)
            {

            }
        }
    }
    public abstract class _Column
    {
        public virtual void Print(int i) { }
        public virtual int GetSize() { return -1; }
        public virtual object GetValue(int i) { return null; }
        public virtual void AddRow(object d) { }
    }

    public class _Column<T> : _Column where T : _Types
    {
        public string Name;
        LinkedList<T> Rows;

        public _Column(string Name)
        {
            this.Name = Name;
            Rows = new LinkedList<T>();
        }

        public override void Print(int Row)
        {
            Console.Out.Write(Rows.ElementAt(Row).GetData());
        }

        public override int GetSize()
        {
            return Rows.Count();
        }

        public override object GetValue(int Row)
        {
            return Rows.ElementAt(Row);
        }

        public override void AddRow(object Data)
        {
            Rows.AddLast((T)Data);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            _Relation DB = new _Relation("DBTest");

            DB.AddColumn(new _Column<_Types._VarChar>("Name"));
            DB.AddColumn(new _Column<_Types._Int>("Age"));

            DB.AddRow(new _Types[] { new _Types._VarChar("Shrek"), new _Types._Int(69) });
            DB.AddRow(new _Types[] { new _Types._VarChar("asdf"), new _Types._Int(69) });

            DB.Print();

        }
    }
}
